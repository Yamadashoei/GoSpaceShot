#pragma once
#include "KamataEngine.h"
#include "kMath.h"

class HitFlash {
public:
	// 画面に出す赤フラッシュ
	void Initialize(uint32_t whiteTex, const KamataEngine::Vector3& worldPos, float durationSec, float startRadiusPx, float endRadiusPx,
	                float peakAlpha = 1.0f); // 濃さ

	void Update(const KamataEngine::Camera& cam, float dt);
	void Draw();

	bool IsAlive() const { return alive_; }
	void SetWorldPos(const KamataEngine::Vector3& wp) { world_ = wp; }

private:
	uint32_t tex_ = 0;
	KamataEngine::Sprite* spr_ = nullptr;

	// 対象のワールド位置
	KamataEngine::Vector3 world_{0, 0, 0};

	// 再生パラメータ
	float dur_ = 0.2f;
	float t_ = 0.0f;         // 経過
	float r0_ = 24.0f;       // 始半径[px]
	float r1_ = 96.0f;       // 終半径[px]
	float peakAlpha_ = 1.0f; // 最大不透明度を上げる

	// スクリーン座標の保存
	KamataEngine::Vector2 screen_{0, 0};

	bool alive_ = false;

	// 投影補助
	static KamataEngine::Matrix4x4 MakePerspectiveFovLH_(float fovY, float aspect, float zn, float zf);
	static KamataEngine::Matrix4x4 MakeViewFromCameraTR_(const KamataEngine::Camera& cam);
	static float FovY_();
};
