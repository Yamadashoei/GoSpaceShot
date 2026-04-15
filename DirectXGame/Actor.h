#pragma once
#include "KamataEngine.h"

class Actor {
public:
	virtual ~Actor() = default;

	// インターフェース
	virtual void Update() = 0;
	virtual void Draw(KamataEngine::Camera& cam) = 0;

	// データの取得
	virtual const KamataEngine::Vector3& GetPosition() const = 0;
	virtual float GetRadius() const = 0;

	virtual void Damage(int d) = 0;
	virtual bool IsDead() const = 0;
	virtual int GetHP() const = 0;
	virtual int GetMaxHP() const = 0;
};