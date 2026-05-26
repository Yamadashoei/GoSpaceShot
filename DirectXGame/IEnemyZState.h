#pragma once
#include "KamataEngine.h"

class Enemy;

class IEnemyZState {
public:
	virtual ~IEnemyZState() = default;

	virtual void Enter(Enemy& enemy) { (void)enemy; }
	virtual void Update(Enemy& enemy, const KamataEngine::Vector3& playerPos, float deltaSec) = 0;
};