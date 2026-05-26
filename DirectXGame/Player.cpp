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
	actionState_ = ActionState::Normal;

	inputLeft_ = false;
	inputRight_ = false;
	inputUp_ = false;
	inputDown_ = false;
	inputAttackTrigger_ = false;
}

void Player::SetPosition(const Vector3& pos) {
	wt_.translation_ = pos;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Player::Update() {
	HandleInput_();
	UpdateState_();
	UpdateByState_();
	UpdateBullets_();
	UpdateSpeed_();
	UpdateMatrix_();
}

void Player::HandleInput_() {
	inputLeft_ = input_->PushKey(DIK_A);
	inputRight_ = input_->PushKey(DIK_D);
	inputUp_ = input_->PushKey(DIK_W);
	inputDown_ = input_->PushKey(DIK_S);
	inputAttackTrigger_ = input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0);
}

void Player::UpdateState_() {
	if (inputAttackTrigger_) {
		actionState_ = ActionState::Attack;
	} else {
		actionState_ = ActionState::Normal;
	}
}

void Player::UpdateByState_() {
	switch (actionState_) {
	case ActionState::Normal:
		UpdateNormalState_();
		break;
	case ActionState::Attack:
		UpdateAttackState_();
		break;
	}
}

void Player::UpdateNormalState_() {
	UpdateMove_();
	UpdateRoll_();
}

void Player::UpdateAttackState_() {
	UpdateMove_();
	UpdateRoll_();
	FireBullet_();
}

void Player::UpdateMove_() {
	if (inputLeft_) {
		wt_.translation_.x -= moveSpeed_;
	}
	if (inputRight_) {
		wt_.translation_.x += moveSpeed_;
	}
	if (inputUp_) {
		wt_.translation_.y += moveSpeed_;
	}
	if (inputDown_) {
		wt_.translation_.y -= moveSpeed_;
	}
}

void Player::UpdateRoll_() {
	float targetRoll = 0.0f;
	if (inputLeft_ && !inputRight_) {
		targetRoll = +rollMaxRad_;
	} else if (inputRight_ && !inputLeft_) {
		targetRoll = -rollMaxRad_;
	}
	rollRad_ += (targetRoll - rollRad_) * rollLerp_;
	wt_.rotation_.z = rollRad_;
}

void Player::FireBullet_() {
	Vector3 spawn = wt_.translation_;
	spawn.z += 0.6f;
	Vector3 vel = {0.0f, 0.0f, bulletSpeed_};
	auto& b = bullets_.emplace_back();
	b.Initialize(spawn, vel);
}

void Player::UpdateBullets_() {
	for (auto& b : bullets_) {
		b.Update();
	}
	bullets_.remove_if([](const PlayerBullet& b) { return b.IsDead(); });
}

void Player::UpdateSpeed_() {
	const Vector3 d = {wt_.translation_.x - prevPos_.x, wt_.translation_.y - prevPos_.y, wt_.translation_.z - prevPos_.z};

	speedFrame_ = std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
	prevPos_ = wt_.translation_;
}

void Player::UpdateMatrix_() {
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Player::Draw(Camera& cam) {
	if (model_) {
		model_->Draw(wt_, cam);
	}
	for (auto& b : bullets_) {
		b.Draw(cam);
	}
}