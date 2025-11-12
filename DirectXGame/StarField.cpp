#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "StarField.h"
#include "kMath.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>

using namespace KamataEngine;

static inline float Rand01() { return float(std::rand() % 10000) / 10000.0f; }
static inline float RandRange(float a, float b) { return a + (b - a) * Rand01(); }


static Matrix4x4 MakePerspectiveFovLH_(float fovY, float aspect, float zn, float zf) {
	Matrix4x4 m{};
	const float f = 1.0f / std::tan(fovY * 0.5f);
	m.m[0][0] = f / aspect;
	m.m[1][1] = f;
	m.m[2][2] = zf / (zf - zn);
	m.m[2][3] = 1.0f;
	m.m[3][2] = (-zn * zf) / (zf - zn);
	return m;
}
static Matrix4x4 MakeViewFromCameraTR_(const Camera& cam) {
	Matrix4x4 Tinv = MakeTranslateMatrix(Vector3{-cam.translation_.x, -cam.translation_.y, -cam.translation_.z});
	Matrix4x4 Rinv = Multiply(MakeRotateZMatrix(-cam.rotation_.z), Multiply(MakeRotateYMatrix(-cam.rotation_.y), MakeRotateXMatrix(-cam.rotation_.x)));
	return Multiply(Tinv, Rinv);
}

float StarField::FovY_() { return 60.0f * 3.1415926535f / 180.0f; }


void StarField::Initialize(uint32_t tex, int count, float spreadX, float spreadY, float zNear, float zFar) {
	tex_ = tex;
	count_ = (count > 0) ? count : 1;
	spreadX_ = spreadX;
	spreadY_ = spreadY;
	zNear_ = (zNear > 0.1f) ? zNear : 12.0f;
	zFar_ = (zFar > zNear_) ? zFar : (zNear_ + 30.0f);

	stars_.resize(count_);
	for (int i = 0; i < count_; ++i) {
		Vector2 pos2D(0, 0);
		Vector4 col(1, 1, 1, 1);
		stars_[static_cast<std::size_t>(i)].spr = Sprite::Create(tex_, pos2D, col);
		stars_[static_cast<std::size_t>(i)].spr->SetAnchorPoint({0.5f, 0.5f});
		stars_[static_cast<std::size_t>(i)].baseSizePx = RandRange(2.0f, 5.0f);
		stars_[static_cast<std::size_t>(i)].spr->SetSize({stars_[static_cast<std::size_t>(i)].baseSizePx, stars_[static_cast<std::size_t>(i)].baseSizePx});
	}
	pure2D_ = false;
	screenMode_ = false;
}


void StarField::ResetAround(const Vector3& center) {
	pure2D_ = false;
	screenMode_ = false;
	for (auto& s : stars_) {
		s.world = {center.x + RandRange(-spreadX_, +spreadX_), center.y + RandRange(-spreadY_, +spreadY_), center.z + RandRange(zNear_, zFar_)};
	}
}

void StarField::ResetInView(const Camera& cam, int screenW, int screenH, float zNear, float zFar, float marginPx) {
	pure2D_ = false;
	screenMode_ = true;
	screenW_ = screenW;
	screenH_ = screenH;
	marginPx_ = marginPx;
	zNear_ = zNear;
	zFar_ = zFar;

	const Matrix4x4 view = MakeViewFromCameraTR_(cam);
	const float aspect = float(screenW_) / float(screenH_);
	const Matrix4x4 proj = MakePerspectiveFovLH_(FovY_(), aspect, 0.1f, 1000.0f);

	for (auto& s : stars_) {
		float d = RandRange(zNear_, zFar_);
		s.world = RandomWorldInView_(cam, d);
		float sz = s.baseSizePx * (zNear_ / (0.5f * zNear_ + d));
		sz = std::max(1.5f, sz);
		s.spr->SetSize({sz, sz});
		Vector2 sp = WorldToScreen(s.world, view, proj, screenW_, screenH_);
		s.spr->SetPosition(sp);
	}
}

void StarField::UsePureScreenSpace(bool enable) { pure2D_ = enable; }

void StarField::ResetScreenSpace(int screenW, int screenH, float sizeNearPx, float sizeFarPx, float marginPx) {
	pure2D_ = true;
	screenW_ = screenW;
	screenH_ = screenH;
	marginPx_ = marginPx;

	for (auto& s : stars_) {
		s.depth01 = Rand01(); // 0は近い(大きい/速い), 1は遠い(小さい/遅い)
		s.screen.x = RandRange(-marginPx_, float(screenW_) + marginPx_);
		s.screen.y = RandRange(-marginPx_, float(screenH_) + marginPx_);

		float sz = std::clamp(sizeFarPx + (sizeNearPx - sizeFarPx) * (1.0f - s.depth01), sizeFarPx, sizeNearPx);
		s.spr->SetSize({sz, sz});
		s.spr->SetPosition(s.screen);
	}
}


void StarField::Update(const Camera& cam, float dt, float /*scrollDz*/, const Vector3& /*playerPos*/, float intensity) {
	if (pure2D_) {
		Update2D_(dt, intensity);
		return;
	}

	const Matrix4x4 view = MakeViewFromCameraTR_(cam);
	const float aspect = float(screenW_) / float(screenH_);
	const Matrix4x4 proj = MakePerspectiveFovLH_(FovY_(), aspect, 0.1f, 1000.0f);

	const float carryZ = 0.0f;
	const float extra = 0.0f;

	for (std::size_t i = 0; i < stars_.size(); ++i) {
		auto& s = stars_[i];

		s.world.z -= (carryZ + extra);

		if (s.world.z < cam.translation_.z + 0.5f) {
			RespawnAhead3D_(i, cam);
			continue;
		}

		Vector2 sp = WorldToScreen(s.world, view, proj, screenW_, screenH_);
		if (sp.x < -marginPx_ * 2 || sp.x > screenW_ + marginPx_ * 2 || sp.y < -marginPx_ * 2 || sp.y > screenH_ + marginPx_ * 2) {
			RespawnAhead3D_(i, cam);
			continue;
		}
		s.spr->SetPosition(sp);
	}
}

void StarField::Update2D_(float dt, float intensity) {
	const Vector2 center(float(screenW_) * 0.5f, float(screenH_) * 0.5f);

	for (std::size_t i = 0; i < stars_.size(); ++i) {
		auto& s = stars_[i];

		// 画面中心から外向きへ流す
		Vector2 dir = {s.screen.x - center.x, s.screen.y - center.y};
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		if (len < 1e-3f) {
			float a = RandRange(0.0f, 6.2831853f);
			dir = {std::cos(a), std::sin(a)};
		} else {
			dir.x /= len;
			dir.y /= len;
		}

		const float basePx = 180.0f; // px/s
		float speed = basePx * (1.0f + 1.2f * intensity) * (0.35f + 0.65f * (1.0f - s.depth01));

		float jitterA = RandRange(-0.05f, 0.05f);
		Vector2 n = {-dir.y, dir.x};

		s.screen.x += (dir.x * speed + n.x * speed * jitterA) * dt;
		s.screen.y += (dir.y * speed + n.y * speed * jitterA) * dt;

		// 画面外に出たらランダムで再投入
		if (s.screen.x < -marginPx_ || s.screen.x > float(screenW_) + marginPx_ || s.screen.y < -marginPx_ || s.screen.y > float(screenH_) + marginPx_) {
			Respawn2D_(i);
			continue;
		}

		s.spr->SetPosition(s.screen);
	}
}

void StarField::Respawn2D_(std::size_t idx) {
	stars_[idx].depth01 = Rand01();
	stars_[idx].screen.x = RandRange(-marginPx_, float(screenW_) + marginPx_);
	stars_[idx].screen.y = RandRange(-marginPx_, float(screenH_) + marginPx_);
	stars_[idx].spr->SetPosition(stars_[idx].screen);
}

// 3D互換ユーティリティ
Vector3 StarField::RandomWorldInView_(const Camera& cam, float depth) const {
	const float aspect = float(screenW_) / float(screenH_);
	const float fovY = FovY_();
	const float halfH = std::tan(fovY * 0.5f) * depth;
	const float halfW = halfH * aspect;

	const float marginWorldX = (marginPx_ / float(screenW_)) * (halfW * 2.0f);
	const float marginWorldY = (marginPx_ / float(screenH_)) * (halfH * 2.0f);

	float x = cam.translation_.x + RandRange(-halfW - marginWorldX, +halfW + marginWorldX);
	float y = cam.translation_.y + RandRange(-halfH - marginWorldY, +halfH + marginWorldY);
	float z = cam.translation_.z + depth; // +Zが前

	return {x, y, z};
}

void StarField::RespawnAhead3D_(std::size_t idx, const Camera& cam) {
	float d = RandRange(zFar_ * 0.7f, zFar_);
	stars_[idx].world = RandomWorldInView_(cam, d);

	float sz = stars_[idx].baseSizePx * (zNear_ / (0.5f * zNear_ + d));
	sz = std::max(1.5f, sz);
	stars_[idx].spr->SetSize({sz, sz});

	const Matrix4x4 view = MakeViewFromCameraTR_(cam);
	const float aspect = float(screenW_) / float(screenH_);
	const Matrix4x4 proj = MakePerspectiveFovLH_(FovY_(), aspect, 0.1f, 1000.0f);
	Vector2 sp = WorldToScreen(stars_[idx].world, view, proj, screenW_, screenH_);
	stars_[idx].spr->SetPosition(sp);
}

void StarField::Draw() {
	for (auto& s : stars_)
		if (s.spr)
			s.spr->Draw();
}
