#pragma once
#include "IScene.h"
#include "KamataEngine.h"
#include "SceneState.h"

class GameClear : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	bool IsNextSceneRequested() const override { return next_; }
	SceneState GetNextScene() const override { return nextScene_; }

	~GameClear();

private:
	KamataEngine::DirectXCommon* dx_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* sprBG_ = nullptr;
	KamataEngine::Sprite* sprClear_ = nullptr;

	bool next_ = false;
	SceneState nextScene_ = SceneState::Title;
};