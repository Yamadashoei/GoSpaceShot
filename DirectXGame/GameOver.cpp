#include "GameOver.h"
#include <base/TextureManager.h>
using namespace KamataEngine;

GameOver::~GameOver() {
	delete sprBG_;
	delete sprOver_;
}

void GameOver::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	sprOver_ = Sprite::Create(TextureManager::Load("./Resources/Scene/gameover.png"), {640.0f, 360.0f});
	sprOver_->SetAnchorPoint({0.5f, 0.5f});

	next_ = false;
	nextScene_ = SceneState::Title;
}

void GameOver::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		next_ = true;
		nextScene_ = SceneState::Title;
	}
}

void GameOver::Draw() {
	ID3D12GraphicsCommandList* cmd = dx_->GetCommandList();
	Sprite::PreDraw(cmd);
	if (sprBG_)
		sprBG_->Draw();
	if (sprOver_)
		sprOver_->Draw();
	Sprite::PostDraw();
	dx_->ClearDepthBuffer();
}
