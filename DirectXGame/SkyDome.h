#pragma once
#include "KamataEngine.h"

class SkyDome {
public:
	~SkyDome();

	void Initialize();
	void Update();
	void Draw(KamataEngine::Camera& viewProjection);

	//中心位置をセット
	void SetCenter(const KamataEngine::Vector3& p);
	void FollowCamera(const KamataEngine::Camera& cam) { SetCenter(cam.translation_); }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
};
