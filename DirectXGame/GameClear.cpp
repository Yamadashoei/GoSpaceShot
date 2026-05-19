#include "GameClear.h"
#include <base/TextureManager.h>
using namespace KamataEngine;

GameClear::~GameClear() {
	delete sprBG_;
	delete sprClear_;
}

void GameClear::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	sprClear_ = Sprite::Create(TextureManager::Load("./Resources/Scene/clear.png"), {640.0f, 360.0f});
	sprClear_->SetAnchorPoint({0.5f, 0.5f});

	next_ = false;
	nextScene_ = SceneState::Title;
}

void GameClear::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		next_ = true;
		nextScene_ = SceneState::Title;
	}
}

void GameClear::Draw() {
	ID3D12GraphicsCommandList* cmd = dx_->GetCommandList();
	Sprite::PreDraw(cmd);
	if (sprBG_)
		sprBG_->Draw();
	if (sprClear_)
		sprClear_->Draw();
	Sprite::PostDraw();
	dx_->ClearDepthBuffer();
}