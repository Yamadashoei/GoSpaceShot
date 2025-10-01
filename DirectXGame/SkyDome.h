#pragma once
#include "KamataEngine.h"

class SkyDome {

public: 

	~SkyDome();
	
	void Initialize();

	void Update();

	void Draw(KamataEngine::Camera& viewProjection);

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
};
