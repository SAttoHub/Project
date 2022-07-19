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
public:
	RenderTarget();
	~RenderTarget();

	void Initialize(DXGI_FORMAT RTV_Format, bool IsUseDepth);
	void PreDraw();
	void PostDraw();

	int GetRenderTexture() { return m_RTV_Texture; }
	int GetDepthTexture();
};

