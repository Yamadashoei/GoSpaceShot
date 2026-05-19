#include "Title.h"

void Title::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

	sprTitle_ = Sprite::Create(TextureManager::Load("./Resources/Scene/title.png"), {0.0f, 0.0f});

	next_ = false;
	nextScene_ = SceneState::Rule;
}

void Title::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		next_ = true;
		nextScene_ = SceneState::Rule;
	}
}

void Title::Draw() {
	ID3D12GraphicsCommandList* cmd = dx_->GetCommandList();
	Sprite::PreDraw(cmd);
	if (sprBG_)
		sprBG_->Draw();
	if (sprTitle_)
		sprTitle_->Draw();
	Sprite::PostDraw();
	dx_->ClearDepthBuffer();
}