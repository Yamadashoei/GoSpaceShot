#include "EnemyApproachState.h"
#include "Enemy.h"
#include "EnemyNormalState.h"
#include "EnemyRetreatState.h"
#include <cstdlib>
#include <memory>

using namespace KamataEngine;

namespace {
float Rand01_EnemyApproach() { return static_cast<float>(std::rand() % 10000) / 10000.0f; }
float RandRange_EnemyApproach(float a, float b) { return a + (b - a) * Rand01_EnemyApproach(); }
} // namespace

void EnemyApproachState::Enter(Enemy& enemy) {
	(void)enemy;
	timer_ = RandRange_EnemyApproach(0.6f, 1.2f);
}

void EnemyApproachState::Update(Enemy& enemy, const Vector3& playerPos, float deltaSec) {
	timer_ -= deltaSec;
	if (timer_ <= 0.0f) {
		float r = Rand01_EnemyApproach();
		if (r < 0.45f) {
			enemy.ChangeZState(std::make_unique<EnemyNormalState>());
		} else if (r < 0.75f) {
			enemy.ChangeZState(std::make_unique<EnemyApproachState>());
		} else {
			enemy.ChangeZState(std::make_unique<EnemyRetreatState>());
		}
		return;
	}

	float targetGap = enemy.GetDesiredLeadZ();
	enemy.ApplyTargetGap(playerPos, targetGap, deltaSec);
}