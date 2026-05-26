#include "EnemyNormalState.h"
#include "Enemy.h"
#include "EnemyApproachState.h"
#include "EnemyRetreatState.h"
#include <cmath>
#include <cstdlib>
#include <memory>

using namespace KamataEngine;

namespace {
float Rand01_EnemyNormal() { return static_cast<float>(std::rand() % 10000) / 10000.0f; }
float RandRange_EnemyNormal(float a, float b) { return a + (b - a) * Rand01_EnemyNormal(); }
} // namespace

void EnemyNormalState::Enter(Enemy& enemy) {
	(void)enemy;
	timer_ = RandRange_EnemyNormal(0.8f, 1.6f);
}

void EnemyNormalState::Update(Enemy& enemy, const Vector3& playerPos, float deltaSec) {
	timer_ -= deltaSec;
	if (timer_ <= 0.0f) {
		float r = Rand01_EnemyNormal();
		if (r < 0.45f) {
			enemy.ChangeZState(std::make_unique<EnemyNormalState>());
		} else if (r < 0.75f) {
			enemy.ChangeZState(std::make_unique<EnemyApproachState>());
		} else {
			enemy.ChangeZState(std::make_unique<EnemyRetreatState>());
		}
		return;
	}

	float targetGap = enemy.GetDesiredLeadZ() + std::sin((enemy.GetPosition().x + enemy.GetPosition().y) * 0.25f) * 2.0f;

	enemy.ApplyTargetGap(playerPos, targetGap, deltaSec);
}