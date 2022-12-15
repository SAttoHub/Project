#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
#include "TexManager.h"

class RenderTarget {
private:
	ComPtr<ID3D12DescriptorHeap> m_descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> m_descHeapDSV;
	bool m_IsUseDepth;

	int m_RTV_Texture;
	int m_DSV_Texture;
	float m_ClearColor[4];

	DirectX::XMINT2 m_Resolution;
public:
	RenderTarget();
	~RenderTarget();

	void Initialize(DXGI_FORMAT RTV_Format, bool IsUseDepth, DirectX::XMINT2 resolution = { WINDOW_WIDTH, WINDOW_HEIGHT }, DirectX::XMFLOAT4 _ClearColor = XMFLOAT4());
	void PreDraw(bool Clear);
	void PostDraw();

	int GetRenderTexture() { return m_RTV_Texture; }
	int GetDepthTexture();

	DirectX::XMINT2 GetSize() {
		return m_Resolution;
	}
};

