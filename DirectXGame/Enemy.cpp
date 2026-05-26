#include "Enemy.h"
#include "kMath.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace KamataEngine;

namespace {
float Rand01() { return static_cast<float>(std::rand() % 10000) / 10000.0f; }
float RandRange(float a, float b) { return a + (b - a) * Rand01(); }
} // namespace

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
	zState_ = ZState::Normal;
	zStateTimer_ = RandRange(0.8f, 1.6f);
	specialRetreat_ = false;
	specialRetreatGap_ = desiredLeadZ_;

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
	if (left > right) {
		std::swap(left, right);
	}
	moveLeft_ = left;
	moveRight_ = right;
	wt_.translation_.x = std::clamp(wt_.translation_.x, moveLeft_, moveRight_);
}

void Enemy::SetSpeed(float unitsPerSec) { moveSpeedX_ = unitsPerSec; }

// ポリモーフィズム
void Enemy::Update() { Update(wt_.translation_, 1.0f / 60.0f); }

void Enemy::Update(const Vector3& playerPos, float deltaSec) {
	UpdateXMove_(deltaSec);
	UpdateState_(deltaSec);
	UpdateZMove_(playerPos, deltaSec);
	UpdateYMove_(deltaSec);
	UpdateShot_(playerPos, deltaSec);
	UpdateBullets_();
	UpdateMatrix_();
}

void Enemy::UpdateState_(float deltaSec) {
	zStateTimer_ -= deltaSec;
	if (zStateTimer_ > 0.0f) {
		return;
	}

	if (zState_ == ZState::Normal) {
		zState_ = ZState::Special;
		specialRetreat_ = (Rand01() >= 0.5f);

		if (specialRetreat_) {
			zStateTimer_ = RandRange(0.7f, 1.3f);
			specialRetreatGap_ = desiredLeadZ_ + RandRange(retreatExtraMin_, retreatExtraMax_);
		} else {
			zStateTimer_ = RandRange(0.6f, 1.2f);
			specialRetreatGap_ = desiredLeadZ_;
		}
	} else {
		zState_ = ZState::Normal;
		zStateTimer_ = RandRange(0.8f, 1.6f);
	}
}

void Enemy::UpdateXMove_(float deltaSec) {
	wt_.translation_.x += moveSpeedX_ * deltaSec;
	if (wt_.translation_.x <= moveLeft_) {
		wt_.translation_.x = moveLeft_;
		moveSpeedX_ = std::abs(moveSpeedX_);
	}
	if (wt_.translation_.x >= moveRight_) {
		wt_.translation_.x = moveRight_;
		moveSpeedX_ = -std::abs(moveSpeedX_);
	}
}

void Enemy::UpdateZMove_(const Vector3& playerPos, float deltaSec) {
	switch (zState_) {
	case ZState::Normal:
		UpdateZNormal_(playerPos, deltaSec);
		break;
	case ZState::Special:
		UpdateZSpecial_(playerPos, deltaSec);
		break;
	}
}

void Enemy::UpdateZNormal_(const Vector3& playerPos, float deltaSec) {
	float targetGap = desiredLeadZ_ + std::sin((wt_.translation_.x + wt_.translation_.y) * 0.25f) * 2.0f;
	ApplyTargetGap_(playerPos, targetGap, deltaSec);
}

void Enemy::UpdateZSpecial_(const Vector3& playerPos, float deltaSec) {
	float targetGap = specialRetreat_ ? specialRetreatGap_ : desiredLeadZ_;
	ApplyTargetGap_(playerPos, targetGap, deltaSec);
}

void Enemy::ApplyTargetGap_(const Vector3& playerPos, float targetGap, float deltaSec) {
	float gap = wt_.translation_.z - playerPos.z;
	float t = std::clamp(zCohesionRate_ * deltaSec, 0.0f, 1.0f);
	gap += (targetGap - gap) * t;
	wt_.translation_.z = playerPos.z + gap;
}

void Enemy::UpdateYMove_(float deltaSec) {
	yRetargetTimer_ -= deltaSec;
	if (yRetargetTimer_ <= 0.0f) {
		yTarget_ = RandRange(yRangeMin_, yRangeMax_);
		yRetargetTimer_ = RandRange(yRetargetIntervalMin_, yRetargetIntervalMax_);
	}

	float dy = yTarget_ - wt_.translation_.y;
	float step = yLerpRate_ * deltaSec;
	if (std::fabs(dy) <= step) {
		wt_.translation_.y = yTarget_;
	} else {
		wt_.translation_.y += (dy > 0.0f ? step : -step);
	}
}

void Enemy::UpdateShot_(const Vector3& playerPos, float deltaSec) {
	shotTimerSec_ += deltaSec;
	if (shotTimerSec_ < shotIntervalSec_) {
		return;
	}

	shotTimerSec_ = 0.0f;
	Vector3 dir = Subtract(playerPos, wt_.translation_);
	if (float len2 = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z; len2 < 1e-6f) {
		dir = {0, 0, -1};
	} else {
		dir = Normalize(dir);
	}

	Vector3 spawn = wt_.translation_ + dir * 0.6f;
	Vector3 vel = dir * bulletSpeed_;
	auto& b = bullets_.emplace_back();
	b.Initialize(model_, spawn, vel);
}

void Enemy::UpdateBullets_() {
	for (auto& b : bullets_) {
		b.Update();
	}
	bullets_.remove_if([](const EnemyBullet& b) { return b.IsDead(); });
}

void Enemy::UpdateMatrix_() {
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Enemy::Draw(Camera& camera) {
	if (model_) {
		model_->Draw(wt_, camera);
	}
	for (auto& b : bullets_) {
		b.Draw(camera);
	}
}