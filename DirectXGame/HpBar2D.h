#pragma once
#include "KamataEngine.h"

class HpBar2D {
public:
	void Initialize(uint32_t whiteTex, const KamataEngine::Vector2& screenPos, const KamataEngine::Vector2& size, const KamataEngine::Vector4& fillColor = {0.0f, 1.0f, 0.0f, 1.0f});
	void SetRatio(float r); // 0.0f ~ 1.0f
	void Draw();

private:
	KamataEngine::Sprite* back_ = nullptr;
	KamataEngine::Sprite* fill_ = nullptr;
	KamataEngine::Vector2 size_{};
};
