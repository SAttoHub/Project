#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

#include "PostGraphPrimitive.h"

class Gaussian
{
private:
	//定数バッファ用構造体
	struct ConstBufferDataGaussian {
		float Weight0;
		float Weight1;
		float Weight2;
		float Weight3;
		float Weight4;
		float Weight5;
		float Weight6;
		float Weight7;

		float Width;
		float Height;
		float OffsetX;
		float OffsetY;
	};
	float weight[8];
	//定数バッファ
	ComPtr<ID3D12Resource> ConstBuffGaussian;

	struct ConstBufferData {
		XMMATRIX mat; //変換行列
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

private:
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	ComPtr<ID3D12Resource> TextureBuff;
public:
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	int GaussNum = 0; // 0 : 横　1 : 縦
	float Dispersion = 0.03f; // ガウスの強さ

	void Initialize(int GaussNumber, float Power);

	const float clearColor[4] = { 0.9f, 0.9f, 0.9f, 1.0f };

	void Draw(ID3D12DescriptorHeap *Descriptor);

	void ClearDepth();

	void PreDrawScene(int Num);
	void PostDrawScene();
};

