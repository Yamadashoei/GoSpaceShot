#pragma once
#include "EnemyBullet.h"
#include "KamataEngine.h"
#include <list>

class Enemy {
public:
	
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
	void SetPosition(const KamataEngine::Vector3& pos);

	void Update(const KamataEngine::Vector3& playerPos, float deltaSec = 1.0f / 60.0f);
	void Draw(KamataEngine::Camera& camera);


	void SetMoveBounds(float left, float right); // 左右の往復範囲
	void SetSpeed(float unitsPerSec);            // X移動速度

	// 弾アクセス
	std::list<EnemyBullet>& GetBullets() { return bullets_; }

	// 位置・判定・HP
	const KamataEngine::Vector3& GetPosition() const { return wt_.translation_; }
	float GetRadius() const { return radius_; }

	void Damage(int d) { hp_ = (hp_ - d < 0) ? 0 : hp_ - d; }
	bool IsDead() const { return hp_ <= 0; }
	int GetHP() const { return hp_; }
	int GetMaxHP() const { return 300; }

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;

	// --- 移動まわり ---
	float moveLeft_ = -12.0f;
	float moveRight_ = +12.0f;
	float moveSpeedX_ = +6.0f; 

	// 発射まわり
	float shotTimerSec_ = 0.0f;
	float shotIntervalSec_ = 1.0f; // 5秒ごと
	float bulletSpeed_ = 0.6f;

	// 弾
	std::list<EnemyBullet> bullets_;

	// 判定・HP
	float radius_ = 1.2f;
	int hp_ = 300;
};
