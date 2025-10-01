#include "Player.h"
#include "kMath.h"
#include <input/Input.h> 
using namespace KamataEngine;

void Player::Initialize(Model* model) {
	model_ = model;
	input_ = Input::GetInstance();

	wt_.Initialize();
	// カメラが z=-10 
	wt_.translation_ = {0.0f, 0.0f, 0.0f};
	wt_.UpdateMatrix();
	wt_.TransferMatrix();

	bullets_.clear();
}

void Player::SetPosition(const Vector3& pos) {
	wt_.translation_ = pos;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Player::Update() {
	
	if (input_->PushKey(DIK_A))
		wt_.translation_.x -= moveSpeed_;
	if (input_->PushKey(DIK_D))
		wt_.translation_.x += moveSpeed_;
	if (input_->PushKey(DIK_W))
		wt_.translation_.y += moveSpeed_;
	if (input_->PushKey(DIK_S))
		wt_.translation_.y -= moveSpeed_;

	// 発射（SPACE または 左クリック）
	const bool trig = input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0);
	if (trig) {
		// 自機のちょい前（Z+）から発射
		KamataEngine::Vector3 spawn = wt_.translation_;
		spawn.z += 0.6f; 

		// 画面の奥へ（+Z 方向）
		KamataEngine::Vector3 vel = {0.0f, 0.0f, bulletSpeed_};

		auto& b = bullets_.emplace_back();
		b.Initialize(spawn, vel); }


	//弾の更新＆寿命で削除
	for (auto& b : bullets_)
		b.Update();
	bullets_.remove_if([](const PlayerBullet& b) { return b.IsDead(); });

	// 行列更新
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Player::Draw(Camera& cam) {
	if (model_)
		model_->Draw(wt_, cam);
	for (auto& b : bullets_)
		b.Draw(cam);
}
