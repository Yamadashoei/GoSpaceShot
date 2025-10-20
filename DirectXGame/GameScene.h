#pragma once
#include "KamataEngine.h"

#include "Enemy.h"
#include "HpBar2D.h"
#include "HpBarBillboard.h"
#include "Player.h"
#include "SceneState.h"
#include "SkyDome.h"

#include <algorithm>
#include <list>

using namespace KamataEngine;

class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

	bool IsNextSceneRequested() const { return next_; }
	SceneState GetNextScene() const { return nextScene_; }

private:
	// 基盤
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// カメラ
	Camera camera_;
	float cameraBaseZ_ = -10.0f;
	float cameraZNow_ = -10.0f;

	// モデル
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;

	// 実体
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	SkyDome* skydome_ = nullptr;

	// シーン遷移
	bool next_ = false;
	SceneState nextScene_ = SceneState::Title;

	// HPバー / テクスチャ
	uint32_t whiteTex_ = 0;
	HpBar2D playerHpUI_;
	HpBarBillboard enemyHpUI_;

	// スピードライン
	struct SpeedLine {
		Sprite* spr = nullptr;
		Vector3 worldPos{0, 0, 0};
		float worldLen = 3.0f;
		float velZ = -12.0f; // カメラへ向かう
		float thicknessPx = 3.0f;
		float life = 0.0f;
		float lifeInit = 0.0f;
	};
	std::list<SpeedLine> speedLines_;
	float lineEmitAccum_ = 0.0f;

	// 演出パラメータ
	float maxSpeedFX_ = 0.30f;
	float dashPullback_ = 2.2f;
	float vignetteMaxA_ = 0.35f;

	// 全体前進（リグ移動）
	float railBaseSpeed_ = 6.0f;        // [wu/s]
	float railBoostMax_ = 10.0f;        // 操作強度で上乗せ
	float lastScrollDz_ = 0.0f;         // 今フレームの前進量
	float worldTravelZ_ = 0.0f;         // 積算（カメラ追従に利用）
	void ApplyForwardMotion_(float dz); // Player/Enemy/弾をZ+へ

private:
	// 当たり判定まとめ
	void HandleCollisions();

	// 球判定
	static bool SphereHit(const Vector3& a, float ra, const Vector3& b, float rb);

	// 2D演出
	void EmitSpeedLines_(float intensity);
	void UpdateSpeedLines_(float dt);
	void DrawSpeedLines_();
	void DrawVignette_(float intensity);
};
