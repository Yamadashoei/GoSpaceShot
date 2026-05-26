#include "EnemyRetreatState.h"
#include "Enemy.h"
#include "EnemyApproachState.h"
#include "EnemyNormalState.h"
#include <cstdlib>
#include <memory>

using namespace KamataEngine;

namespace {
float Rand01_EnemyRetreat() { return static_cast<float>(std::rand() % 10000) / 10000.0f; }
float RandRange_EnemyRetreat(float a, float b) { return a + (b - a) * Rand01_EnemyRetreat(); }
} // namespace

void EnemyRetreatState::Enter(Enemy& enemy) {
	(void)enemy;
	timer_ = RandRange_EnemyRetreat(0.7f, 1.3f);
	retreatGap_ = enemy.GetDesiredLeadZ() + RandRange_EnemyRetreat(enemy.GetRetreatExtraMin(), enemy.GetRetreatExtraMax());
}

void EnemyRetreatState::Update(Enemy& enemy, const Vector3& playerPos, float deltaSec) {
	timer_ -= deltaSec;
	if (timer_ <= 0.0f) {
		float r = Rand01_EnemyRetreat();
		if (r < 0.45f) {
			enemy.ChangeZState(std::make_unique<EnemyNormalState>());
		} else if (r < 0.75f) {
			enemy.ChangeZState(std::make_unique<EnemyApproachState>());
		} else {
			enemy.ChangeZState(std::make_unique<EnemyRetreatState>());
		}
		return;
	}

	enemy.ApplyTargetGap(playerPos, retreatGap_, deltaSec);
}