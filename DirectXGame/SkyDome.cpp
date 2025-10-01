#include "Skydome.h"
#include "cassert"

using namespace KamataEngine;

SkyDome::~SkyDome() { delete model_; }

void SkyDome::Initialize() {
	// 引数で受け取ったデータをメンバ変数に記録
	model_ = Model::CreateFromOBJ("skydome");
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void SkyDome::Update() {
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void SkyDome::Draw(Camera& viewProjection) { model_->Draw(worldTransform_, viewProjection); }
