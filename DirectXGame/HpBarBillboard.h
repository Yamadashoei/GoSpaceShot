#pragma once
#include "KamataEngine.h"
#include "kMath.h" 

class HpBarBillboard {
public:
	void Initialize(uint32_t whiteTex, const KamataEngine::Vector2& size = {100.0f, 8.0f}, const KamataEngine::Vector3& worldOffset = {0.0f, 2.2f, 0.0f});
	// ワールド位置とカメラ、HPを渡してスクリーン座標に変換
	void Update(const KamataEngine::Vector3& worldPos, const KamataEngine::Camera& cam, int hp, int maxHp, int screenW, int screenH);
	void Draw();

private:
	KamataEngine::Sprite* back_ = nullptr;
	KamataEngine::Sprite* fill_ = nullptr;
	KamataEngine::Vector2 size_{};
	KamataEngine::Vector3 worldOffset_{};
};
