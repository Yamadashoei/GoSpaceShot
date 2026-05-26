#pragma once
#include "IScene.h"
#include "KamataEngine.h"

#include "Enemy.h"
#include "HpBar2D.h"
#include "HpBarBillboard.h"
#include "IGamePlayState.h"
#include "Player.h"
#include "SceneState.h"
#include "SkyDome.h"
#include "StarField.h"

#include <algorithm>
#include <list>
#include <memory>

using namespace KamataEngine;

class GameScene : public IScene {
public:
	~GameScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	bool IsNextSceneRequested() const override { return next_; }
	SceneState GetNextScene() const override { return nextScene_; }

	// ===== GameState用公開関数 =====
	Input* GetInput() const { return input_; }
	void ChangePlayState(std::unique_ptr<IGamePlayState> newState);
	void UpdatePlayingCore();
	void DrawPauseOverlay(int pauseIndex);
	void RequestTitleScene();

private:
	void HandleCollisions();
	static bool SphereHit(const Vector3& a, float ra, const Vector3& b, float rb);

	void EmitSpeedLines_(float intensity);
	void UpdateSpeedLines_(float dt);
	void DrawSpeedLines_();
	void DrawVignette_(float intensity);

	void TriggerShake(float amp, float duration, float freq = 22.0f);
	void UpdateShake_(float dt, Vector3& camTranslateIO);

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	std::unique_ptr<IGamePlayState> playState_;

	Camera camera_;
	float cameraBaseZ_ = -10.0f;
	float cameraZNow_ = -10.0f;

	float shakeTimer_ = 0.0f;
	float shakeDuration_ = 0.0f;
	float shakeAmp_ = 0.0f;
	float shakeFreq_ = 22.0f;
	unsigned int shakeSeed_ = 0u;

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	SkyDome* skydome_ = nullptr;

	bool next_ = false;
	SceneState nextScene_ = SceneState::Title;

	uint32_t whiteTex_ = 0;
	HpBar2D playerHpUI_;
	HpBarBillboard enemyHpUI_;

	bool enableSpeedLines_ = false;

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

	float maxSpeedFX_ = 0.30f;
	float dashPullback_ = 2.2f;
	float vignetteMaxA_ = 0.35f;

	float railBaseSpeed_ = 6.0f;
	float railBoostMax_ = 10.0f;
	float lastScrollDz_ = 0.0f;
	float worldTravelZ_ = 0.0f;
	void ApplyForwardMotion_(float dz);

	StarField starFar_;
	StarField starNear_;

	float enemyDesiredLeadZ_ = 20.0f;
	float enemyZNowVisual_ = 0.0f;
	float enemyCohesionLerp_ = 0.12f;
	float enemyDragOnBoost_ = 3.0f;
};