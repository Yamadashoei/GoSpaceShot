#pragma once
#include "IEnemyZState.h"

class EnemyNormalState : public IEnemyZState {
public:
	void Enter(Enemy& enemy) override;
	void Update(Enemy& enemy, const KamataEngine::Vector3& playerPos, float deltaSec) override;

private:
	float timer_ = 0.0f;
};