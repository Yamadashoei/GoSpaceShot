#include "HpBar2D.h"
using namespace KamataEngine;

void HpBar2D::Initialize(uint32_t whiteTex, const Vector2& screenPos, const Vector2& size, const Vector4& fillColor) {
	size_ = size;

	back_ = Sprite::Create(whiteTex, screenPos, {0.2f, 0.2f, 0.2f, 1.0f});
	back_->SetAnchorPoint({0.0f, 0.5f});
	back_->SetSize(size_);

	fill_ = Sprite::Create(whiteTex, screenPos, fillColor);
	fill_->SetAnchorPoint({0.0f, 0.5f});
	fill_->SetSize(size_);
}

void HpBar2D::SetRatio(float r) {
	if (!fill_)
		return;
	if (r < 0.0f)
		r = 0.0f;
	if (r > 1.0f)
		r = 1.0f;
	fill_->SetSize({size_.x * r, size_.y});

	// 色をHP割合で少し変化（緑→黄→赤）
	Vector4 col = {(1.0f - r), (r), 0.0f, 1.0f};
	fill_->SetColor(col);
}

void HpBar2D::Draw() {
	if (back_)
		back_->Draw();
	if (fill_)
		fill_->Draw();
}
