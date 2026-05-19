#include "Rule.h"

void Rule::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

	sprRule_ = Sprite::Create(TextureManager::Load("./Resources/Scene/rule.png"), {0.0f, 0.0f});

	next_ = false;
	nextScene_ = SceneState::Game;
}

void Rule::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		next_ = true;
		nextScene_ = SceneState::Game;
	}
}

void Rule::Draw() {
	ID3D12GraphicsCommandList* cmd = dx_->GetCommandList();
	Sprite::PreDraw(cmd);
	if (sprBG_)
		sprBG_->Draw();
	if (sprRule_)
		sprRule_->Draw();
	Sprite::PostDraw();
	dx_->ClearDepthBuffer();
}