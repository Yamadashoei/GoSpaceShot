#pragma once
#include "KamataEngine.h"

#include "Enemy.h"
#include "Player.h"
#include "SceneState.h"

#include "SkyDome.h"

#include "HpBar2D.h"
#include "HpBarBillboard.h"


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
	
	// HPバー
	 uint32_t whiteTex_ = 0;
	HpBar2D playerHpUI_;
	HpBarBillboard enemyHpUI_;

private:
	// 当たり判定まとめ
	void HandleCollisions();

	// 球判定
	static bool SphereHit(const Vector3& a, float ra, const Vector3& b, float rb);
};
