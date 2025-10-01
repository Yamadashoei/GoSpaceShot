#include "HpBarBillboard.h"
using namespace KamataEngine;

void HpBarBillboard::Initialize(uint32_t whiteTex, const Vector2& size, const Vector3& worldOffset) {
	size_ = size;
	worldOffset_ = worldOffset;

	back_ = Sprite::Create(whiteTex, {0, 0}, {0.2f, 0.2f, 0.2f, 1.0f});
	// 中央 → 左端へ
	back_->SetAnchorPoint({0.0f, 0.5f});
	back_->SetSize(size_);

	fill_ = Sprite::Create(whiteTex, {0, 0}, {0.0f, 1.0f, 0.0f, 1.0f});
	// 中央 → 左端へ
	fill_->SetAnchorPoint({0.0f, 0.5f});
	fill_->SetSize(size_);
}

void HpBarBillboard::Update(const Vector3& worldPos, const Camera& cam, int hp, int maxHp, int screenW, int screenH) {
	if (!back_ || !fill_)
		return;

	const Vector3 head = {worldPos.x + worldOffset_.x, worldPos.y + worldOffset_.y, worldPos.z + worldOffset_.z};

	Vector2 sp = WorldToScreen(head, cam.matView, cam.matProjection, screenW, screenH);

	// バーの「左端」
	float leftX = sp.x - size_.x * 0.5f;
	float y = sp.y;

	back_->SetPosition({leftX, y});

	float r = (maxHp > 0) ? (float)hp / (float)maxHp : 0.0f;
	if (r < 0.0f)
		r = 0.0f;
	if (r > 1.0f)
		r = 1.0f;

	fill_->SetPosition({leftX, y});
	fill_->SetSize({size_.x * r, size_.y});
	fill_->SetColor({(1.0f - r), r, 0.0f, 1.0f});
}


void HpBarBillboard::Draw() {
	if (back_)
		back_->Draw();
	if (fill_)
		fill_->Draw();
}
