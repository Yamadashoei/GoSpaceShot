#pragma once
#include "IEnemyZState.h"

class EnemyRetreatState : public IEnemyZState {
public:
	void Enter(Enemy& enemy) override;
	void Update(Enemy& enemy, const KamataEngine::Vector3& playerPos, float deltaSec) override;

private:
	float timer_ = 0.0f;
	float retreatGap_ = 0.0f;
};