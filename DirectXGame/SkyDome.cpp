#include "SkyDome.h"

void SkyDome::Initialize() {
	model_ = Model::CreateFromOBJ("cube"); 
	wt_.Initialize();
	wt_.scale_ = {100.0f, 100.0f, 100.0f};
	wt_.UpdateMatrix();
}

void SkyDome::Draw(Camera& cam) {
	if (model_)
		model_->Draw(wt_, cam);
}
