#include "SkyDome.h"
#include <cassert>

using namespace KamataEngine;

SkyDome::~SkyDome() { delete model_; }

void SkyDome::Initialize() {
	model_ = Model::CreateFromOBJ("skydome");
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void SkyDome::Update() {
	// 位置は GameScene 側から SetCenter で更新する
}

void SkyDome::SetCenter(const Vector3& p) {
	worldTransform_.translation_ = p;
	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void SkyDome::Draw(Camera& viewProjection) {
	if (model_)
		model_->Draw(worldTransform_, viewProjection);
}
