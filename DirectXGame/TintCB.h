#pragma once
#include <d3d12.h>

struct alignas(256) TintCBData {
	float tint[4]; // r,g,b,a
	float pad[60]; // 256B アラインのための余白
};

class TintCB {
public:
	// PS の b3 に対応する「ルートパラメータの index」を設定（あなたの環境値）
	static void SetRootIndex(UINT idx);

	static void Init(ID3D12Device* device);
	static void Shutdown();

	static void Set(float r, float g, float b, float a = 1.0f);
	static void Bind(ID3D12GraphicsCommandList* cmd);
	static void SetWhiteAndBind(ID3D12GraphicsCommandList* cmd) {
		Set(1.f, 1.f, 1.f, 1.f);
		Bind(cmd);
	}

private:
	static UINT s_rootIndex; // 例: 3 or 4 など
	static ID3D12Resource* s_res;
	static TintCBData* s_ptr;
};
