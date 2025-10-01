#pragma once
#include "KamataEngine.h"
#include "SceneState.h"
using namespace KamataEngine;

class Rule {
public:
	void Initialize();
	void Update();
	void Draw();
	bool IsNextSceneRequested() const { return next_; }
	SceneState GetNextScene() const { return nextScene_; }

private:
	DirectXCommon* dx_ = nullptr;
	Input* input_ = nullptr;
	Camera camera_;

	Sprite* sprBG_ = nullptr;
	Sprite* sprRule_ = nullptr;

	bool next_ = false;
	SceneState nextScene_ = SceneState::Game;
};
