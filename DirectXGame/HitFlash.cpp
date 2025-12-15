#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "HitFlash.h"
#include <algorithm>
#include <cmath>

using namespace KamataEngine;

static inline float Clamp01(float v) { return (v < 0.f) ? 0.f : (v > 1.f) ? 1.f : v; }

float HitFlash::FovY_() { return 60.0f * 3.1415926535f / 180.0f; }

Matrix4x4 HitFlash::MakePerspectiveFovLH_(float fovY, float aspect, float zn, float zf) {
	Matrix4x4 m{};
	const float f = 1.0f / std::tan(fovY * 0.5f);
	m.m[0][0] = f / aspect;
	m.m[1][1] = f;
	m.m[2][2] = zf / (zf - zn);
	m.m[2][3] = 1.0f;
	m.m[3][2] = (-zn * zf) / (zf - zn);
	return m;
}
Matrix4x4 HitFlash::MakeViewFromCameraTR_(const Camera& cam) {
	Matrix4x4 Tinv = MakeTranslateMatrix(Vector3{-cam.translation_.x, -cam.translation_.y, -cam.translation_.z});
	Matrix4x4 Rinv = Multiply(MakeRotateZMatrix(-cam.rotation_.z), Multiply(MakeRotateYMatrix(-cam.rotation_.y), MakeRotateXMatrix(-cam.rotation_.x)));
	return Multiply(Tinv, Rinv);
}

void HitFlash::Initialize(uint32_t whiteTex, const Vector3& worldPos, float durationSec, float startRadiusPx, float endRadiusPx, float peakAlpha) {
	tex_ = whiteTex;
	world_ = worldPos;
	dur_ = std::max(0.01f, durationSec);
	t_ = 0.0f;
	r0_ = startRadiusPx;
	r1_ = endRadiusPx;
	peakAlpha_ = peakAlpha;

	// 中心基準で円板を描く（白1x1を拡大）
	Vector2 pos(0, 0);
	Vector4 col(1, 0, 0, 1); // 赤
	spr_ = Sprite::Create(tex_, pos, col);
	spr_->SetAnchorPoint({0.5f, 0.5f});

	alive_ = true;
}

void HitFlash::Update(const Camera& cam, float dt) {
	if (!alive_)
		return;

	t_ += dt;
	float u = Clamp01(t_ / dur_); // 0→1

	// 半径補間（線形）
	float radius = r0_ + (r1_ - r0_) * u;

	// 不透明度カーブ：序盤強め保持→後半急減衰
	// 例：alphaBase = (1-u)^2 を少し強め、ピークで倍率
	float alphaBase = (1.0f - u);
	alphaBase = alphaBase * alphaBase; // 二乗で急減衰
	float alpha = Clamp01(alphaBase * peakAlpha_);

	// ワールド→スクリーン投影
	const int SW = WinApp::kWindowWidth;
	const int SH = WinApp::kWindowHeight;
	const float aspect = float(SW) / float(SH);
	const Matrix4x4 view = MakeViewFromCameraTR_(cam);
	const Matrix4x4 proj = MakePerspectiveFovLH_(FovY_(), aspect, 0.1f, 1000.0f);

	screen_ = WorldToScreen(world_, view, proj, SW, SH);

	// セット
	spr_->SetPosition(screen_);
	spr_->SetSize({radius * 2.0f, radius * 2.0f}); // 直径で拡大
	spr_->SetColor({1.0f, 0.0f, 0.0f, alpha});     // 濃い赤（R=1,G=0,B=0）×ピーク

	if (u >= 1.0f) {
		alive_ = false;
	}
}

void HitFlash::Draw() {
	if (alive_ && spr_)
		spr_->Draw();
}
