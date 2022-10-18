#include "RenderTarget.h"
#include <cassert>

RenderTarget::RenderTarget()
{
	m_ClearColor[0] = 0.0f;
	m_ClearColor[1] = 0.0f;
	m_ClearColor[2] = 0.0f;
	m_ClearColor[3] = 1.0f;
	m_RTV_Texture = 0;
	m_DSV_Texture = 0;
	m_IsUseDepth = false;
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Initialize(DXGI_FORMAT RTV_Format, bool IsUseDepth, DirectX::XMINT2 resolution)
{
	m_Resolution = resolution;
	m_IsUseDepth = IsUseDepth;
	// 生成成功判定用
	HRESULT result;

	// RTV用テクスチャ生成
	m_RTV_Texture = TexManager::GetPostTexture(m_Resolution.x, m_Resolution.y, XMFLOAT4(0.0f, 0.0f, 0.0f, 255.0f), RTV_Format);
	// RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	// RTV用デスクリプタヒープ生成
	result = DirectXBase::dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&m_descHeapRTV));
	assert(SUCCEEDED(result));
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = RTV_Format;
	// デスクリプタヒープにRTV作成
	DirectXBase::dev->CreateRenderTargetView(TexManager::TextureData[m_RTV_Texture].TextureBuff.Get(),
		&rtvDesc,
		m_descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	if (m_IsUseDepth) {
		// DSV用テクスチャ生成
		m_DSV_Texture = TexManager::GetPostDepthTexture(m_Resolution.x, m_Resolution.y, XMFLOAT4(0.0f, 0.0f, 0.0f, 255.0f));
		// DSV用デスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
		DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DescHeapDesc.NumDescriptors = 1;
		// DSV用デスクリプタヒープを作成
		result = DirectXBase::dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&m_descHeapDSV));
		assert(SUCCEEDED(result));
		// デスクリプタヒープにDSV作成
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		DirectXBase::dev->CreateDepthStencilView(TexManager::TextureData[m_DSV_Texture].TextureBuff.Get(),
			&dsvDesc,
			m_descHeapDSV->GetCPUDescriptorHandleForHeapStart());
	}
}

void RenderTarget::PreDraw()
{
	//リソースバリアを変更
	DirectXBase::cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(TexManager::TextureData[m_RTV_Texture].TextureBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	// Depthを使用する場合
	if (m_IsUseDepth) {
		//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_descHeapDSV->GetCPUDescriptorHandleForHeapStart();
		DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	}
	// Depthを使用しない場合
	else {
		DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	}
	//ビューポートの設定
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		m_Resolution.x, m_Resolution.y));
	//シザリング矩形の設定
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, m_Resolution.x, m_Resolution.y));
	//全画面クリア
	DirectXBase::cmdList->ClearRenderTargetView(rtvH, m_ClearColor, 0, nullptr);
	//深度バッファのクリア
	if (m_IsUseDepth) {
		//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_descHeapDSV->GetCPUDescriptorHandleForHeapStart();
		DirectXBase::cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
}

void RenderTarget::PostDraw()
{
	DirectXBase::cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(TexManager::TextureData[m_RTV_Texture].TextureBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

int RenderTarget::GetDepthTexture()
{
	if (!m_IsUseDepth) {
		assert(("Message: This render target does not use depth", 0));
	}
	return m_DSV_Texture;
}


/*------------コメントアウト消去前コード*/
/*
	void RenderTarget::PreDraw()
	{
		//リソースバリアを変更
		DirectXBase::cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(TexManager::TextureData[TexNum].TextureBuff.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
		//DirectXBase::cmdList->ResourceBarrier(1,
		//	&CD3DX12_RESOURCE_BARRIER::Transition(TexManager::TextureData[TexNum2].TextureBuff.Get(),
		//		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		//		D3D12_RESOURCE_STATE_RENDER_TARGET));


	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//for (int i = 0; i < Num; i++) {
	//	rtvH.ptr += DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//}
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, &descHeapDSV->GetCPUDescriptorHandleForHeapStart());
	//DirectXBase::cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	//ビューポートの設定
	DirectXBase::cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		WINDOW_WIDTH, WINDOW_HEIGHT));
	//シザリング矩形の設定
	DirectXBase::cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	//全画面クリア
	DirectXBase::cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	//DirectXBase::cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	DirectXBase::cmdList->ClearDepthStencilView(descHeapDSV->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}

	void RenderTarget::PostDraw()
	{
		DirectXBase::cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(TexManager::TextureData[TexNum].TextureBuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
		//DirectXBase::cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(TexManager::TextureData[TexNum2].TextureBuff.Get(),
		//	D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	}
*/