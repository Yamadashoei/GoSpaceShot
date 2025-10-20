#pragma once
#include "Collision.h"
#include "KamataEngine.h"

class EnemyBullet {
public:
	void Initialize(KamataEngine::Model*, const KamataEngine::Vector3& pos, const KamataEngine::Vector3& vel);
	void Initialize(const KamataEngine::Vector3& pos);
	void Initialize(const KamataEngine::Vector3& pos, const KamataEngine::Vector3& vel);

	void Update();
	void Draw(const KamataEngine::Camera& vp);

	bool IsDead() const { return isDead_; }
	bool IsAlive() const { return !isDead_; }
	int GetDamage() const { return 50; }

	const KamataEngine::Vector3& GetPos() const { return wt_.translation_; }
	float GetRadius() const { return collision_.GetRadius(); }
	const Collision& GetCollision() const { return collision_; }

	// Zに加算
	void AddScrollZ(float dz);

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Vector3 vel_{};

	static constexpr int kLife = 60 * 5;
	int life_ = kLife;
	bool isDead_ = false;

	Collision collision_;
	static KamataEngine::Model* sModel_;
};
