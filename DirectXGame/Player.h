#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include "Actor.h"
#include "Collision.h"
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <algorithm>
#include <list>

class Player : public Actor {
public:
	void Initialize(KamataEngine::Model* model);
	void SetPosition(const KamataEngine::Vector3& pos);
	void SetMoveSpeed(float s) { moveSpeed_ = s; }

	void Update() override;
	void Draw(KamataEngine::Camera& cam) override;

	// 弾
	std::list<PlayerBullet>& GetBullets() { return bullets_; }

	const KamataEngine::Vector3& GetPosition() const override { return wt_.translation_; }
	float GetRadius() const override { return radius_; }

	// 現在フレームの移動量
	float GetSpeed() const { return speedFrame_; }

	// HP
	void Damage(int d) override { hp_ = (std::max)(0, hp_ - d); }
	bool IsDead() const override { return hp_ <= 0; }
	int GetHP() const override { return hp_; }
	int GetMaxHP() const override { return maxHP_; }

	// 最大HP/現在HPの設定
	void SetMaxHP(int v, bool refill = true) {
		maxHP_ = (std::max)(1, v);
		if (refill)
			hp_ = maxHP_;
		else
			hp_ = (std::min)(hp_, maxHP_);
	}
	void SetHP(int v) { hp_ = (std::clamp)(v, 0, maxHP_); }

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	float moveSpeed_ = 0.2f;

	// 弾
	std::list<PlayerBullet> bullets_;
	float bulletSpeed_ = 0.8f;

	// 当たり判定
	float radius_ = 1.0f;

	// HP
	int maxHP_ = 100;
	int hp_ = 100;

	// スピード
	KamataEngine::Vector3 prevPos_{0, 0, 0};
	float speedFrame_ = 0.0f;

	// A/D で傾ける
	float rollRad_ = 0.0f;
	const float rollMaxRad_ = 3.14f / 4.0f;
	const float rollLerp_ = 0.2f;
};