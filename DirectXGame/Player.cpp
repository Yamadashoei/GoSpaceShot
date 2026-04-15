#include "Player.h"
#include "kMath.h"
#include <cmath>
#include <input/Input.h>

using namespace KamataEngine;

void Player::Initialize(Model* model) {
	model_ = model;
	input_ = Input::GetInstance();

	wt_.Initialize();
	wt_.translation_ = {0.0f, 0.0f, 0.0f};
	wt_.rotation_ = {0.0f, 0.0f, 0.0f};
	wt_.UpdateMatrix();
	wt_.TransferMatrix();

	bullets_.clear();

	// HPは最大値で満タン
	hp_ = maxHP_;

	// スピード初期化
	prevPos_ = wt_.translation_;
	speedFrame_ = 0.0f;

	rollRad_ = 0.0f;
}

void Player::SetPosition(const Vector3& pos) {
	wt_.translation_ = pos;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Player::Update() {
	bool left = input_->PushKey(DIK_A);
	bool right = input_->PushKey(DIK_D);

	if (left)
		wt_.translation_.x -= moveSpeed_;
	if (right)
		wt_.translation_.x += moveSpeed_;
	if (input_->PushKey(DIK_W))
		wt_.translation_.y += moveSpeed_;
	if (input_->PushKey(DIK_S))
		wt_.translation_.y -= moveSpeed_;

	float targetRoll = 0.0f;
	if (left && !right)
		targetRoll = +rollMaxRad_;
	else if (right && !left)
		targetRoll = -rollMaxRad_;
	rollRad_ += (targetRoll - rollRad_) * rollLerp_;
	wt_.rotation_.z = rollRad_;

	const bool trig = input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0);
	if (trig) {
		Vector3 spawn = wt_.translation_;
		spawn.z += 0.6f;
		Vector3 vel = {0.0f, 0.0f, bulletSpeed_};
		auto& b = bullets_.emplace_back();
		b.Initialize(spawn, vel);
	}

	for (auto& b : bullets_)
		b.Update();
	bullets_.remove_if([](const PlayerBullet& b) { return b.IsDead(); });

	const Vector3 d = {wt_.translation_.x - prevPos_.x, wt_.translation_.y - prevPos_.y, wt_.translation_.z - prevPos_.z};
	speedFrame_ = std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
	prevPos_ = wt_.translation_;

	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Player::Draw(Camera& cam) {
	if (model_)
		model_->Draw(wt_, cam);
	for (auto& b : bullets_)
		b.Draw(cam);
}