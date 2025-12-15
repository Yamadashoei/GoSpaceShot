#include "Enemy.h"
#include "kMath.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace KamataEngine;

static inline float Rand01() { return static_cast<float>(std::rand() % 10000) / 10000.0f; }
static inline float RandRange(float a, float b) { return a + (b - a) * Rand01(); }

void Enemy::Initialize(Model* model, const Vector3& position) {
	model_ = model;
	wt_.Initialize();
	wt_.translation_ = position;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();

	bullets_.clear();
	shotTimerSec_ = 0.0f;
	hp_ = 300;

	moveLeft_ = -12.0f;
	moveRight_ = +12.0f;
	moveSpeedX_ = +6.0f;

	// Z 初期
	zMode_ = 0;
	zModeDuration_ = RandRange(0.8f, 1.6f);
	zModeTimer_ = zModeDuration_;

	// Y 初期
	yTarget_ = wt_.translation_.y;
	yRetargetTimer_ = RandRange(yRetargetIntervalMin_, yRetargetIntervalMax_);
}

void Enemy::SetPosition(const Vector3& pos) {
	wt_.translation_ = pos;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Enemy::SetMoveBounds(float left, float right) {
	if (left > right)
		std::swap(left, right);
	moveLeft_ = left;
	moveRight_ = right;
	wt_.translation_.x = std::clamp(wt_.translation_.x, moveLeft_, moveRight_);
}

void Enemy::SetSpeed(float unitsPerSec) { moveSpeedX_ = unitsPerSec; }

void Enemy::Update(const Vector3& playerPos, float deltaSec) {
	// X: 往復
	wt_.translation_.x += moveSpeedX_ * deltaSec;
	if (wt_.translation_.x <= moveLeft_) {
		wt_.translation_.x = moveLeft_;
		moveSpeedX_ = std::abs(moveSpeedX_);
	}
	if (wt_.translation_.x >= moveRight_) {
		wt_.translation_.x = moveRight_;
		moveSpeedX_ = -std::abs(moveSpeedX_);
	}

	// Z: モード切替
	zModeTimer_ -= deltaSec;
	if (zModeTimer_ <= 0.0f) {
		float r = Rand01();
		zMode_ = (r < 0.45f) ? 0 : (r < 0.75f) ? 1 : 2;
		zModeDuration_ = (zMode_ == 0) ? RandRange(0.8f, 1.6f) : (zMode_ == 1) ? RandRange(0.6f, 1.2f) : RandRange(0.7f, 1.3f);
		zModeTimer_ = zModeDuration_;
	}

	float gap = wt_.translation_.z - playerPos.z;
	float targetGap = (zMode_ == 2)   ? (desiredLeadZ_ + RandRange(retreatExtraMin_, retreatExtraMax_))
	                  : (zMode_ == 1) ? desiredLeadZ_
	                                  : desiredLeadZ_ + std::sin((wt_.translation_.x + wt_.translation_.y) * 0.25f) * 2.0f;
	float t = std::clamp(zCohesionRate_ * deltaSec, 0.0f, 1.0f);
	gap += (targetGap - gap) * t;
	wt_.translation_.z = playerPos.z + gap;

	// Y: ランダム追従
	yRetargetTimer_ -= deltaSec;
	if (yRetargetTimer_ <= 0.0f) {
		yTarget_ = RandRange(yRangeMin_, yRangeMax_);
		yRetargetTimer_ = RandRange(yRetargetIntervalMin_, yRetargetIntervalMax_);
	}
	{
		float dy = yTarget_ - wt_.translation_.y;
		float step = yLerpRate_ * deltaSec;
		if (std::fabs(dy) <= step)
			wt_.translation_.y = yTarget_;
		else
			wt_.translation_.y += (dy > 0.0f ? step : -step);
	}

	// 発射：プレイヤー狙い
	shotTimerSec_ += deltaSec;
	if (shotTimerSec_ >= shotIntervalSec_) {
		shotTimerSec_ = 0.0f;
		Vector3 dir = Subtract(playerPos, wt_.translation_);
		if (float len2 = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z; len2 < 1e-6f)
			dir = {0, 0, -1};
		else
			dir = Normalize(dir);

		Vector3 spawn = wt_.translation_ + dir * 0.6f;
		Vector3 vel = dir * bulletSpeed_;
		auto& b = bullets_.emplace_back();
		b.Initialize(model_, spawn, vel);
	}

	for (auto& b : bullets_)
		b.Update();
	bullets_.remove_if([](const EnemyBullet& b) { return b.IsDead(); });

	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Enemy::Draw(Camera& camera) {
	if (model_)
		model_->Draw(wt_, camera);
	for (auto& b : bullets_)
		b.Draw(camera);
}
