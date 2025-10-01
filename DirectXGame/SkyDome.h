#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

class SkyDome {
public:
	void Initialize();
	void Draw(Camera& cam);

private:
	Model* model_ = nullptr;
	WorldTransform wt_{};
};
