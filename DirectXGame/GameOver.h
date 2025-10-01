#pragma once
#include "KamataEngine.h"
#include "SceneState.h"

class GameOver {
public:
	void Initialize();
	void Update();
	void Draw();

	bool IsNextSceneRequested() const { return next_; }
	SceneState GetNextScene() const { return nextScene_; }

	~GameOver();

private:
	KamataEngine::DirectXCommon* dx_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* sprBG_ = nullptr;
	KamataEngine::Sprite* sprOver_ = nullptr;

	bool next_ = false;
	SceneState nextScene_ = SceneState::Title;
};
