#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

#include "PostGraphPrimitive.h"

class DepthOfField
{
private:
	struct ConstBufferData {
		XMMATRIX mat; //変換行列
		float InterpSize; // 最もぼかしが強いところとぼかし無しのところの補間距離
		float Focus; // ぼかし無しの範囲の中心位置
		float FocusSize; // ぼかし無しの範囲
		float Flag;
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

	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
public:
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	int TexNum;

	static float InterpSize;
	static float Focus;
	static float FocusSize;
	static float UseFlag;

	void Initialize(float _InterpSize, float _Focus, float _FocusSize, bool _UseFlag);
	void SetState(float _InterpSize, float _Focus, float _FocusSize, bool _UseFlag);

	const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	void Draw(int TexNum1, int TexNum2, int TexNum3, int TexNum4);

	void ClearDepth();

	void PreDrawScene();
	void PostDrawScene();
};

