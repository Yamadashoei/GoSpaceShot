#pragma once
#include "KamataEngine.h"

class SkyDome {

public: 

	~SkyDome();
	
	void Initialize();

	void Update();

	void Draw(KamataEngine::Camera& viewProjection);

private:                                          // メンバ変数
	KamataEngine::WorldTransform worldTransform_; // ワールド変換データ
	KamataEngine::Model* model_ = nullptr;        // モデル
};
