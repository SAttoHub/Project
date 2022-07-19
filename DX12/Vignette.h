#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

#include "PostGraphPrimitive.h"

class Vignette
{
private:
	struct ConstBufferData {
		XMMATRIX mat; //変換行列
		float UseFlag;
		float Influence;
	};
	ID3DBlob *errorBlob = nullptr;
	//定数バッファ
	ComPtr<ID3D12Resource> ConstBuff0;
private:
	// 画像付き箱プリミティブ専用シェーダー
	ID3DBlob *Graph_vsBlob = nullptr; //画像付き箱プリミティブ用頂点シェーダー
	ID3DBlob *Graph_psBlob = nullptr; //画像付き箱プリミティブ用ピクセルシェーダー
	ID3DBlob *Graph_gsBlob = nullptr; //画像付き箱プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> GraphRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> GraphPipelinestate;
	// 頂点バッファ
	static const int MaxGraphPrimitives = 1; // 画像付き箱プリミティブ最大数
	ComPtr<ID3D12Resource> GraphVertBuff[MaxGraphPrimitives];
	D3D12_VERTEX_BUFFER_VIEW GraphvbView[MaxGraphPrimitives]{};
	// 画像付き箱プリミティブ専用セットアップ
	void SetupGraphPrimitive();
	// 画像付き箱プリミティブデータ
	PostGraphPrimitive GraphData[MaxGraphPrimitives];

	//シェーダーの読み込み
	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);

public:

	void Initialize(float _Influence, bool isUse);
	float UseFlag;
	float Influence;
	void SetState(float _Influence, bool isUse);

	const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	void Draw(int TexNum1);
};

