#pragma once
#include "KamataEngine.h"

#include "Enemy.h"
#include "HpBar2D.h"
#include "HpBarBillboard.h"
#include "Player.h"
#include "SceneState.h"
#include "SkyDome.h"
#include "StarField.h"

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

	// ===== プレイ状態 =====
	enum class PlayState { Playing, Paused };
	PlayState state_ = PlayState::Playing;

	// ポーズ選択 0:再開 1:リスタート 2:タイトル
	int pauseIndex_ = 0;

	// カメラ
	Camera camera_;
	float cameraBaseZ_ = -10.0f;
	float cameraZNow_ = -10.0f;

	// ===== 画面シェイク =====
	void TriggerShake(float amp, float duration, float freq = 22.0f);
	void UpdateShake_(float dt, Vector3& camTranslateIO);
	float shakeTimer_ = 0.0f;     // 残り時間
	float shakeDuration_ = 0.0f;  // 全体時間
	float shakeAmp_ = 0.0f;       // 最大振幅
	float shakeFreq_ = 22.0f;     // 速さ[Hz]
	unsigned int shakeSeed_ = 0u; // 乱数シード

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

	// SpeedLine
	bool enableSpeedLines_ = false;

	// スピードライン
	struct SpeedLine {
		Sprite* spr = nullptr;
		Vector3 worldPos{0, 0, 0};
		float worldLen = 3.0f;
		float velZ = -12.0f;
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

	// 全体前進
	float railBaseSpeed_ = 6.0f;        // [wu/s]
	float railBoostMax_ = 10.0f;        // 操作強度
	float lastScrollDz_ = 0.0f;         // 現在フレームの前進量
	float worldTravelZ_ = 0.0f;         // カメラ追従
	void ApplyForwardMotion_(float dz); // Player/Enemyの弾をZ+へ

	// 星の流れ
	StarField starFar_;
	StarField starNear_;

	// 視覚用パラメータ
	float enemyDesiredLeadZ_ = 20.0f; // プレイヤーの少し前を保つ距離
	float enemyZNowVisual_ = 0.0f;    // 敵の見た目Z
	float enemyCohesionLerp_ = 0.12f; // 追従レート
	float enemyDragOnBoost_ = 3.0f;   // 疾走時の押し戻し量

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

	// ===== ポーズUI =====
	void UpdatePause_();
	void DrawPause_();
};
