#include "EnemyBullet.h"
#include "kMath.h"
using namespace KamataEngine;

Model* EnemyBullet::sModel_ = nullptr;

void EnemyBullet::Initialize(Model*, const Vector3& pos, const Vector3& vel) { Initialize(pos, vel); }
void EnemyBullet::Initialize(const Vector3& pos) { Initialize(pos, Vector3{0.0f, 0.0f, +0.7f}); }

void EnemyBullet::Initialize(const Vector3& pos, const Vector3& vel) {
	if (!sModel_)
		sModel_ = Model::CreateFromOBJ("enemyBullet");
	model_ = sModel_;

	wt_.Initialize();
	wt_.scale_ = {0.8f, 0.8f, 0.8f};
	wt_.translation_ = pos;
	vel_ = vel;

	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();

	collision_.SetPosition(wt_.translation_);
	collision_.SetRadius(0.6f);

	life_ = kLife;
	isDead_ = false;
}

void EnemyBullet::Update() {
	wt_.translation_ += vel_;
	if (--life_ <= 0)
		isDead_ = true;

	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
	collision_.SetPosition(wt_.translation_);
}

void EnemyBullet::Draw(const Camera& vp) {
	if (model_)
		model_->Draw(wt_, vp);
}

void EnemyBullet::AddScrollZ(float dz) {
	wt_.translation_.z += dz;
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
	collision_.SetPosition(wt_.translation_);
}
