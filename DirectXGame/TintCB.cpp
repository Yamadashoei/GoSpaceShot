#include "TintCB.h"
#include <cstring>
#include <d3dx12.h>

UINT TintCB::s_rootIndex = 3; // ★必要に応じて後から SetRootIndex で上書き
ID3D12Resource* TintCB::s_res = nullptr;
TintCBData* TintCB::s_ptr = nullptr;

void TintCB::SetRootIndex(UINT idx) { s_rootIndex = idx; }

void TintCB::Init(ID3D12Device* device) {
	if (s_res)
		return;

	CD3DX12_HEAP_PROPERTIES heap(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(256);

	device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&s_res));

	CD3DX12_RANGE range(0, 0);
	s_res->Map(0, &range, reinterpret_cast<void**>(&s_ptr));
	s_ptr->tint[0] = 1.f;
	s_ptr->tint[1] = 1.f;
	s_ptr->tint[2] = 1.f;
	s_ptr->tint[3] = 1.f;
}

void TintCB::Shutdown() {
	if (s_res) {
		s_res->Unmap(0, nullptr);
		s_ptr = nullptr;
		s_res->Release();
		s_res = nullptr;
	}
}

void TintCB::Set(float r, float g, float b, float a) {
	if (!s_ptr)
		return;
	s_ptr->tint[0] = r;
	s_ptr->tint[1] = g;
	s_ptr->tint[2] = b;
	s_ptr->tint[3] = a;
}

void TintCB::Bind(ID3D12GraphicsCommandList* cmd) {
	if (!s_res)
		return;
	cmd->SetGraphicsRootConstantBufferView(s_rootIndex, s_res->GetGPUVirtualAddress());
}
