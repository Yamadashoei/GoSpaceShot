#pragma once
#include "IScene.h"
#include "KamataEngine.h"
#include "SceneState.h"
using namespace KamataEngine;

class Rule : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool IsNextSceneRequested() const override { return next_; }
	SceneState GetNextScene() const override { return nextScene_; }

private:
	DirectXCommon* dx_ = nullptr;
	Input* input_ = nullptr;
	Camera camera_;

	Sprite* sprBG_ = nullptr;
	Sprite* sprRule_ = nullptr;

	bool next_ = false;
	SceneState nextScene_ = SceneState::Game;
};