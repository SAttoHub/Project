#pragma once

#include "..\..\DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "PrimitiveDatas/BoxPrimitive.h"
#include "PrimitiveDatas/GraphPrimitive.h"
#include "PrimitiveDatas/LinePrimitive.h"

#include "..\..\Singleton.h"

class Primitive2D : public Singleton<Primitive2D> {
	friend class Singleton<Primitive2D>;
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct ConstBufferData {
		XMMATRIX mat; //変換行列
	};
	ComPtr<ID3D12Resource> ConstBuff0;

	ID3DBlob *errorBlob = nullptr;

private: // 箱プリミティブ専用
	// 箱プリミティブ専用シェーダー
	ID3DBlob *Box_vsBlob = nullptr; //箱プリミティブ用頂点シェーダー
	ID3DBlob *Box_psBlob = nullptr; //箱プリミティブ用ピクセルシェーダー
	ID3DBlob *Box_gsBlob = nullptr; //箱プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> BoxRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> BoxPipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> BoxVertBuff;
	D3D12_VERTEX_BUFFER_VIEW BoxvbView{};
	// 箱プリミティブ専用セットアップ
	void SetupBoxPrimitive();
	// 箱プリミティブデータ
	static const int MaxBoxPrimitives = 512; // 箱プリミティブ最大数
	BoxPrimitive BoxData[MaxBoxPrimitives];

private: // 画像付き箱プリミティブ専用
	// 画像付き箱プリミティブ専用シェーダー
	ID3DBlob *Graph_vsBlob = nullptr; //画像付き箱プリミティブ用頂点シェーダー
	ID3DBlob *Graph_psBlob = nullptr; //画像付き箱プリミティブ用ピクセルシェーダー
	ID3DBlob *Graph_gsBlob = nullptr; //画像付き箱プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> GraphRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> GraphPipelinestate;
	// 頂点バッファ
	static const int MaxGraphPrimitives = 512; // 画像付き箱プリミティブ最大数
	ComPtr<ID3D12Resource> GraphVertBuff[MaxGraphPrimitives];
	D3D12_VERTEX_BUFFER_VIEW GraphvbView[MaxGraphPrimitives]{};

	ComPtr<ID3D12Resource> BackGraphVertBuff[MaxGraphPrimitives];
	D3D12_VERTEX_BUFFER_VIEW BackGraphvbView[MaxGraphPrimitives]{};

	// 画像付き箱プリミティブ専用セットアップ
	void SetupGraphPrimitive();
	// 画像付き箱プリミティブデータ
	GraphPrimitive GraphData[MaxGraphPrimitives];

//private: // 線プリミティブ専用
//// 線プリミティブ専用シェーダー
//	ID3DBlob *Line_vsBlob = nullptr; //線プリミティブ用頂点シェーダー
//	ID3DBlob *Line_psBlob = nullptr; //線プリミティブ用ピクセルシェーダー
//	ID3DBlob *Line_gsBlob = nullptr; //線プリミティブ用ジオメトリシェーダー
//	// ルートシグネチャ
//	ComPtr<ID3D12RootSignature> LineRootsignature;
//	// パイプラインステート
//	ComPtr<ID3D12PipelineState> LinePipelinestate;
//	// 頂点バッファ
//	ComPtr<ID3D12Resource> LineVertBuff;
//	D3D12_VERTEX_BUFFER_VIEW LinevbView{};
//	// 線プリミティブ専用セットアップ
//	void SetupLinePrimitive();
//	// 箱プリミティブデータ
//	static const int MaxLinePrimitives = 128; // 線プリミティブ最大数
//	LinePrimitive LineData[MaxLinePrimitives];

public:
	void Initialize();
	void Update();
	void BoxDrawAll();
	//void LineDrawAll();
	void GraphDrawAll();
	void BackGraphDrawAll();
	void BloomGraphDrawAll();
	//void LineDrawAll();
	void Draw();
	void BackDraw();

	void DrawBox(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color);
	void DrawBox(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color, bool Draw3D = false);
	void DrawGraph(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	void DrawGraph(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }, bool Draw3D = false);
	void DrawGraphBack(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	void DrawGraphBack(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }, bool Draw3D = false);
	void DrawGraphBloom(XMFLOAT2 pos1, XMFLOAT2 pos2, int TextureNumber, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	void DrawGraphBloom(XMFLOAT3 pos1, XMFLOAT3 pos2, int TextureNumber, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }, bool Draw3D = false);
	//void DrawLine(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT4 color);
	//void DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color);
	//void DrawGrid(XMFLOAT3 pos1, XMFLOAT3 pos2, float interval, XMFLOAT4 color);

private:
	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
};