#pragma once
#include "KamataEngine.h"
#include <algorithm>
#include <cstddef> 
#include <vector>

class StarField {
public:
	
	void Initialize(uint32_t tex, int count, float spreadX, float spreadY, float zNear, float zFar);

	//中心近傍に撒く
	void ResetAround(const KamataEngine::Vector3& center);

	// 画面全体にばら撒く
	void ResetInView(const KamataEngine::Camera& cam, int screenW, int screenH, float zNear, float zFar, float marginPx);

	// 完全2Dモード
	void UsePureScreenSpace(bool enable);
	void ResetScreenSpace(int screenW, int screenH, float sizeNearPx, float sizeFarPx, float marginPx);


	void Update(const KamataEngine::Camera& cam, float dt, float scrollDz, const KamataEngine::Vector3& /*playerPos*/, float intensity);

	void Draw();

private:
	struct Star {
		KamataEngine::Sprite* spr = nullptr;

		// 3Dモード
		KamataEngine::Vector3 world{0, 0, 0};

		// 2Dモード
		KamataEngine::Vector2 screen{0, 0};
		float depth01 = 0.0f; // 0=近い, 1=遠い

		float baseSizePx = 3.0f;
	};

	uint32_t tex_ = 0;
	std::vector<Star> stars_;
	int count_ = 0;

	// 初期化時のパラメータ
	float spreadX_ = 20.0f, spreadY_ = 12.0f; 
	float zNear_ = 12.0f, zFar_ = 60.0f;

	// モード
	bool pure2D_ = false;    
	bool screenMode_ = false;

	// 画面情報
	int screenW_ = 1280, screenH_ = 720;
	float marginPx_ = 80.0f;

	// 内部ヘルパ
	static float FovY_();
	KamataEngine::Vector3 RandomWorldInView_(const KamataEngine::Camera& cam, float depth) const;

	void RespawnAhead3D_(std::size_t idx, const KamataEngine::Camera& cam);
	void Respawn2D_(std::size_t idx);
	void Update2D_(float dt, float intensity);
};
