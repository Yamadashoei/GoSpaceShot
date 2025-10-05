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
	float cameraBaseZ_ = -10.0f; // ベース位置
	float cameraZNow_ = -10.0f;  // 補間用

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

	struct SpeedLine {
		Sprite* spr = nullptr;     // 2Dスプライト
		Vector3 worldPos{0, 0, 0}; // 始点
		float worldLen = 3.0f;     // ライン長
		float velZ = -12.0f;       // Z速度
		float thicknessPx = 3.0f;  // 太さ
		float life = 0.0f;         // 残寿命
		float lifeInit = 0.0f;     // 初期寿命
	};
	std::list<SpeedLine> speedLines_;
	float lineEmitAccum_ = 0.0f;

	// 演出パラメータ
	float maxSpeedFX_ = 0.30f;   
	float dashPullback_ = 2.2f; 
	float vignetteMaxA_ = 0.35f; // ヴィネット最大暗さ

private:
	// 当たり判定まとめ
	void HandleCollisions();

	// 球判定
	static bool SphereHit(const Vector3& a, float ra, const Vector3& b, float rb);

	// 2D演出ヘルパ
	void EmitSpeedLines_(float intensity); // 生成
	void UpdateSpeedLines_(float dt);      // 移動（Z方向）＋投影
	void DrawSpeedLines_();                // スプライト描画
	void DrawVignette_(float intensity);   // 端の暗幕
};
