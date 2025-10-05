#pragma once
#include "Collision.h"
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	void Initialize(KamataEngine::Model* model);
	void SetPosition(const KamataEngine::Vector3& pos);
	void SetMoveSpeed(float s) { moveSpeed_ = s; }

	void Update();                        // 移動＆弾更新＆発射
	void Draw(KamataEngine::Camera& cam); // 自機と弾の描画

	// 弾
	std::list<PlayerBullet>& GetBullets() { return bullets_; }

	// 位置・判定
	const KamataEngine::Vector3& GetPosition() const { return wt_.translation_; }
	float GetRadius() const { return radius_; }

	// 演出用：今フレームの移動量（長さ）
	float GetSpeed() const { return speedFrame_; }

	// HP
	void Damage(int d) { hp_ = (hp_ - d < 0) ? 0 : hp_ - d; }
	bool IsDead() const { return hp_ <= 0; }
	int GetHP() const { return hp_; }
	int GetMaxHP() const { return 100; }

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	float moveSpeed_ = 0.2f; // キー1フレーム移動量

	// 弾
	std::list<PlayerBullet> bullets_;
	float bulletSpeed_ = 0.8f; // +Z

	// 当たり判定（球）
	float radius_ = 1.0f;

	// HP
	int hp_ = 100;

	// スピード算出
	KamataEngine::Vector3 prevPos_{0, 0, 0};
	float speedFrame_ = 0.0f; // 今フレームの|Δpos|
};
