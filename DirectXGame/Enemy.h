#pragma once
#include "Actor.h"
#include "EnemyBullet.h"
#include "KamataEngine.h"
#include <list>

class Enemy : public Actor {
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
	void SetPosition(const KamataEngine::Vector3& pos);

	void Update(const KamataEngine::Vector3& playerPos, float deltaSec = 1.0f / 60.0f);

	// ポリモーフィズム
	void Update() override;
	void Draw(KamataEngine::Camera& camera) override;

	void SetMoveBounds(float left, float right);
	void SetSpeed(float unitsPerSec);

	// 弾アクセス
	std::list<EnemyBullet>& GetBullets() { return bullets_; }

	// 位置・判定・HP
	const KamataEngine::Vector3& GetPosition() const override { return wt_.translation_; }
	float GetRadius() const override { return radius_; }

	void Damage(int d) override { hp_ = (hp_ - d < 0) ? 0 : hp_ - d; }
	bool IsDead() const override { return hp_ <= 0; }
	int GetHP() const override { return hp_; }
	int GetMaxHP() const override { return 300; }

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;

	float moveLeft_ = -12.0f;
	float moveRight_ = +12.0f;
	float moveSpeedX_ = +6.0f;

	// Z 前後移動
	float desiredLeadZ_ = 18.0f;
	float zModeTimer_ = 0.0f;
	float zModeDuration_ = 0.0f;
	int zMode_ = 0;
	float retreatExtraMin_ = 6.0f;
	float retreatExtraMax_ = 14.0f;
	float zCohesionRate_ = 3.0f;

	// Y ランダム移動
	float yTarget_ = 0.0f;
	float yRetargetTimer_ = 0.0f;
	float yRetargetIntervalMin_ = 0.8f;
	float yRetargetIntervalMax_ = 1.8f;
	float yRangeMin_ = -4.0f;
	float yRangeMax_ = +4.0f;
	float yLerpRate_ = 3.0f;

	// 発射
	float shotTimerSec_ = 0.0f;
	float shotIntervalSec_ = 1.0f;
	float bulletSpeed_ = 0.6f;

	std::list<EnemyBullet> bullets_;

	// 当たり判定
	float radius_ = 1.2f;
	//HP
	int hp_ = 300;
};