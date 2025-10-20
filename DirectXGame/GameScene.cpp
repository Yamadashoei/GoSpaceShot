#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "GameScene.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include <base/DirectXCommon.h>
#include <base/WinApp.h>

#include "kMath.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace KamataEngine;

static inline float Clamp01(float v) { return (v < 0.f) ? 0.f : (v > 1.f) ? 1.f : v; }
static inline float DegToRad(float d) { return d * 3.1415926535f / 180.0f; }

// 左手座標系の透視投影
static Matrix4x4 MakePerspectiveFovLH(float fovY, float aspect, float zn, float zf) {
	Matrix4x4 m{};
	const float f = 1.0f / std::tan(fovY * 0.5f);
	m.m[0][0] = f / aspect;
	m.m[1][1] = f;
	m.m[2][2] = zf / (zf - zn);
	m.m[2][3] = 1.0f;
	m.m[3][2] = (-zn * zf) / (zf - zn);
	return m;
}

// カメラ TR → View
static Matrix4x4 MakeViewFromCameraTR(const Camera& cam) {
	Matrix4x4 Tinv = MakeTranslateMatrix(Vector3{-cam.translation_.x, -cam.translation_.y, -cam.translation_.z});
	Matrix4x4 Rinv = Multiply(MakeRotateZMatrix(-cam.rotation_.z), Multiply(MakeRotateYMatrix(-cam.rotation_.y), MakeRotateXMatrix(-cam.rotation_.x)));
	return Multiply(Tinv, Rinv);
}

bool GameScene::SphereHit(const Vector3& a, float ra, const Vector3& b, float rb) {
	const float dx = a.x - b.x;
	const float dy = a.y - b.y;
	const float dz = a.z - b.z;
	const float rr = (ra + rb) * (ra + rb);
	return (dx * dx + dy * dy + dz * dz) < rr;
}

GameScene::~GameScene() {
	delete player_;
	delete enemy_;
	delete modelPlayer_;
	delete modelEnemy_;
	delete skydome_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// カメラ
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, cameraBaseZ_};
	camera_.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_.UpdateMatrix();
	cameraZNow_ = cameraBaseZ_;
	worldTravelZ_ = 0.0f;
	lastScrollDz_ = 0.0f;

	// モデル
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelEnemy_ = Model::CreateFromOBJ("enemy");

	// 実体
	player_ = new Player();
	player_->Initialize(modelPlayer_);
	player_->SetPosition({0.0f, 0.0f, 20.0f});

	enemy_ = new Enemy();
	enemy_->Initialize(modelEnemy_, {0.0f, 0.0f, 40.0f});

	// 遷移
	next_ = false;
	nextScene_ = SceneState::Title;

	// UI
	whiteTex_ = TextureManager::Load("./Resources/white1x1.png");
	playerHpUI_.Initialize(whiteTex_, {30.0f, 30.0f}, {220.0f, 18.0f});
	enemyHpUI_.Initialize(whiteTex_, {100.0f, 8.0f}, {0.0f, 3.0f, 0.0f});

	// スカイドーム
	skydome_ = new SkyDome();
	skydome_->Initialize();

	// 演出
	speedLines_.clear();
	lineEmitAccum_ = 0.0f;
}

void GameScene::Update() {
	const float dt = 1.0f / 60.0f;

	// 更新
	player_->Update();
	enemy_->Update(player_->GetPosition(), dt);

	// 衝突
	HandleCollisions();

	if (!next_) {
		if (enemy_->IsDead()) {
			next_ = true;
			nextScene_ = SceneState::GameClear;
		} else if (player_->IsDead()) {
			next_ = true;
			nextScene_ = SceneState::GameOver;
		}
	}

	// HPバー
	playerHpUI_.SetRatio(static_cast<float>(player_->GetHP()) / player_->GetMaxHP());
	enemyHpUI_.Update(enemy_->GetPosition(), camera_, enemy_->GetHP(), enemy_->GetMaxHP(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// スカイドーム
	skydome_->Update();

	// 疾走
	float intensity = Clamp01(player_->GetSpeed() / maxSpeedFX_);

	// Z+へ前進（プレイヤー/敵/弾）
	{
		float railSpeed = railBaseSpeed_ + railBoostMax_ * intensity; // [wu/s]
		lastScrollDz_ = railSpeed * dt;
		worldTravelZ_ += lastScrollDz_;
		ApplyForwardMotion_(lastScrollDz_);
	}

	// カメラ
	{
		float targetZ = (cameraBaseZ_ + worldTravelZ_) - dashPullback_ * intensity;
		cameraZNow_ = cameraZNow_ * 0.88f + targetZ * 0.12f;
		camera_.translation_.z = cameraZNow_;
		camera_.UpdateMatrix();

		// 
		skydome_->SetCenter(camera_.translation_);
	}

	// スピードライン
	EmitSpeedLines_(intensity);
	UpdateSpeedLines_(dt);
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* cmd = dxCommon_->GetCommandList();

	// 3D
	Model::PreDraw();
	enemy_->Draw(camera_);
	player_->Draw(camera_);
	skydome_->Draw(camera_);
	Model::PostDraw();

	// 2D
	Sprite::PreDraw(cmd);
	{
		DrawSpeedLines_();

		float intensity = Clamp01(player_->GetSpeed() / maxSpeedFX_);
		DrawVignette_(intensity);

		playerHpUI_.Draw();
		enemyHpUI_.Draw();
	}
	Sprite::PostDraw();

	dxCommon_->ClearDepthBuffer();
}

void GameScene::HandleCollisions() {
	auto& pBullets = player_->GetBullets();
	auto& eBullets = enemy_->GetBullets();

	for (auto it = pBullets.begin(); it != pBullets.end();) {
		if (SphereHit(it->GetPos(), it->GetRadius(), enemy_->GetPosition(), enemy_->GetRadius())) {
			enemy_->Damage(it->GetDamage());
			it = pBullets.erase(it);
		} else {
			++it;
		}
	}

	for (auto it = eBullets.begin(); it != eBullets.end();) {
		if (SphereHit(it->GetPos(), it->GetRadius(), player_->GetPosition(), player_->GetRadius())) {
			player_->Damage(it->GetDamage());
			it = eBullets.erase(it);
		} else {
			++it;
		}
	}

	// 弾同士
	for (auto pit = pBullets.begin(); pit != pBullets.end();) {
		bool eraseP = false;
		for (auto eit = eBullets.begin(); eit != eBullets.end();) {
			if (SphereHit(pit->GetPos(), pit->GetRadius(), eit->GetPos(), eit->GetRadius())) {
				eit = eBullets.erase(eit);
				eraseP = true;
				break;
			} else {
				++eit;
			}
		}
		if (eraseP)
			pit = pBullets.erase(pit);
		else
			++pit;
	}
}

// 全体前進Z+
void GameScene::ApplyForwardMotion_(float dz) {
	// Player
	{
		Vector3 p = player_->GetPosition();
		p.z += dz;
		player_->SetPosition(p);
	}
	// Enemy
	{
		Vector3 e = enemy_->GetPosition();
		e.z += dz;
		enemy_->SetPosition(e);
	}
	// Bullets
	for (auto& b : player_->GetBullets()) {
		b.AddScrollZ(dz);
	}
	for (auto& b : enemy_->GetBullets()) {
		b.AddScrollZ(dz);
	}
}

// スピードライン生成
void GameScene::EmitSpeedLines_(float intensity) {
	const float rate = 20.0f * intensity;
	lineEmitAccum_ += rate * (1.0f / 60.0f);

	while (lineEmitAccum_ >= 1.0f) {
		lineEmitAccum_ -= 1.0f;

		SpeedLine L;

		Vector2 pos2D(0.0f, 0.0f);
		Vector4 col(1.0f, 1.0f, 1.0f, 0.9f);
		L.spr = Sprite::Create(whiteTex_, pos2D, col);
		L.spr->SetAnchorPoint({0.5f, 0.0f});

		const Vector3 p = player_->GetPosition();
		float offX = ((std::rand() % 1000) / 1000.0f - 0.5f) * 4.0f;
		float offY = ((std::rand() % 1000) / 1000.0f - 0.5f) * 2.0f;
		float offZ = 6.0f + (std::rand() % 1000) / 1000.0f * 10.0f;
		L.worldPos = {p.x + offX, p.y + offY, p.z + offZ};

		L.worldLen = 2.5f + (std::rand() % 1000) / 1000.0f * 3.0f;
		L.thicknessPx = 2.0f + (std::rand() % 1000) / 1000.0f * 3.0f;
		L.velZ = -(6.0f + 24.0f * intensity); // カメラ方向へ
		L.lifeInit = L.life = 0.35f + 0.35f * intensity;

		speedLines_.push_back(L);
	}
}

// スピードライン更新
void GameScene::UpdateSpeedLines_(float dt) {
	const int SW = WinApp::kWindowWidth;
	const int SH = WinApp::kWindowHeight;

	const Matrix4x4 view = MakeViewFromCameraTR(camera_);
	const float fovY = DegToRad(60.0f);
	const float aspect = static_cast<float>(SW) / static_cast<float>(SH);
	const Matrix4x4 proj = MakePerspectiveFovLH(fovY, aspect, 0.1f, 1000.0f);
	const Matrix4x4 viewProj = Multiply(view, proj);

	for (auto it = speedLines_.begin(); it != speedLines_.end();) {
		it->life -= dt;
		it->worldPos.z += it->velZ * dt;

		
		it->worldPos.z -= lastScrollDz_;

		if (it->life <= 0.0f || it->worldPos.z < camera_.translation_.z + 0.5f) {
			it = speedLines_.erase(it);
			continue;
		}

		Vector3 A = it->worldPos;
		Vector3 B = it->worldPos;
		B.z -= it->worldLen;

		// カリング
		{
			Vector4 clipA = Transform(Vector4{A.x, A.y, A.z, 1.0f}, viewProj);
			Vector4 clipB = Transform(Vector4{B.x, B.y, B.z, 1.0f}, viewProj);
			if (clipA.w <= 0.0f || clipB.w <= 0.0f) {
				it = speedLines_.erase(it);
				continue;
			}
		}

		Vector2 As = WorldToScreen(A, view, proj, SW, SH);
		Vector2 Bs = WorldToScreen(B, view, proj, SW, SH);

		float dx = Bs.x - As.x;
		float dy = Bs.y - As.y;
		float lenPx = std::sqrt(dx * dx + dy * dy);
		if (!(lenPx > 0.5f)) {
			++it;
			continue;
		}

		float angle = std::atan2(dy, dx);
		it->spr->SetPosition(As);
		it->spr->SetSize({it->thicknessPx, lenPx});
		it->spr->SetRotation(angle);

		float t = (it->lifeInit > 0.0f) ? (it->life / it->lifeInit) : 0.0f;
		float alpha = (std::max)(0.0f, t);
		it->spr->SetColor({1.0f, 1.0f, 1.0f, alpha});

		++it;
	}
}

void GameScene::DrawSpeedLines_() {
	for (auto& l : speedLines_) {
		if (l.spr)
			l.spr->Draw();
	}
}

void GameScene::DrawVignette_(float intensity) {
	if (intensity <= 0.001f)
		return;

	const float W = static_cast<float>(WinApp::kWindowWidth);
	const float H = static_cast<float>(WinApp::kWindowHeight);
	const float thick = 80.0f;
	const float alpha = vignetteMaxA_ * intensity;

	auto drawBand = [&](float x, float y, float w, float h) {
		Vector2 pos(0.0f, 0.0f);
		Vector4 col(0.0f, 0.0f, 0.0f, alpha);
		Sprite* s = Sprite::Create(whiteTex_, pos, col);
		s->SetAnchorPoint({0.0f, 0.0f});
		s->SetPosition({x, y});
		s->SetSize({w, h});
		s->Draw();
	};

	drawBand(0.0f, 0.0f, W, thick);
	drawBand(0.0f, H - thick, W, thick);
	drawBand(0.0f, 0.0f, thick, H);
	drawBand(W - thick, 0.0f, thick, H);
}
