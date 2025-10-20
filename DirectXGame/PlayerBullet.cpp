#include "PlayerBullet.h"
#include "kMath.h"
using namespace KamataEngine;

Model* PlayerBullet::sModel_ = nullptr;

void PlayerBullet::Initialize(Model*, const Vector3& pos, const Vector3& vel) { Initialize(pos, vel); }

void PlayerBullet::Initialize(const Vector3& pos, const Vector3& vel) {
	if (!sModel_)
		sModel_ = Model::CreateFromOBJ("playerBullet");
	model_ = sModel_;

	wt_.Initialize();
	wt_.scale_ = {0.6f, 0.6f, 0.6f};
	wt_.translation_ = pos;
	vel_ = vel;

	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();

	collision_.SetPosition(wt_.translation_);
	collision_.SetRadius(0.5f);

	life_ = kLife;
	isDead_ = false;
}

void PlayerBullet::Update() {
	wt_.translation_ += vel_;
	if (--life_ <= 0)
		isDead_ = true;

	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
	collision_.SetPosition(wt_.translation_);
}

void PlayerBullet::Draw(const Camera& vp) {
	if (model_)
		model_->Draw(wt_, vp);
}

void PlayerBullet::AddScrollZ(float dz) {
	wt_.translation_.z += dz;
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
	collision_.SetPosition(wt_.translation_);
}
