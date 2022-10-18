#pragma once

#include "..\..\DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "PrimitiveDatas/CubePrimitive3D.h"
#include "PrimitiveDatas/LinePrimitive3D.h"
#include "PrimitiveDatas/OctahedronPrimitive.h"
#include "PrimitiveDatas/IcosahedronPrimitive.h"
#include "PrimitiveDatas/TetrahedronPrimitive.h"


#include "..\..\Singleton.h"

class Primitive3D : public Singleton<Primitive3D>{
	friend class Singleton<Primitive3D>;
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct ConstBufferData {
		XMMATRIX mat; //3D変換行列
		XMMATRIX matBillboard;
	};
	ComPtr<ID3D12Resource> ConstBuff0;

	ID3DBlob *errorBlob = nullptr;

private: // 箱プリミティブ専用
	// 箱プリミティブ専用シェーダー
	ID3DBlob *Cube_vsBlob = nullptr; //箱プリミティブ用頂点シェーダー
	ID3DBlob *Cube_psBlob = nullptr; //箱プリミティブ用ピクセルシェーダー
	ID3DBlob *Cube_gsBlob = nullptr; //箱プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CubeRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> CubePipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> CubeVertBuff;
	D3D12_VERTEX_BUFFER_VIEW CubevbView{};
	// 箱プリミティブ専用セットアップ
	void SetupCubePrimitive();
	// 箱プリミティブデータ
	static const int MaxCubePrimitives = 32; // 箱プリミティブ最大数
	CubePrimitive3D CubeData[MaxCubePrimitives];

private: // Lineプリミティブ専用
	// 線プリミティブ専用シェーダー
	ID3DBlob *Line_vsBlob = nullptr; //箱プリミティブ用頂点シェーダー
	ID3DBlob *Line_psBlob = nullptr; //箱プリミティブ用ピクセルシェーダー
	ID3DBlob *Line_gsBlob = nullptr; //箱プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> LineRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> LinePipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> LineVertBuff;
	D3D12_VERTEX_BUFFER_VIEW LinevbView{};
	// 線プリミティブ専用セットアップ
	void SetupLinePrimitive();
	// 線プリミティブデータ
	static const int MaxLinePrimitives = 512; // 線プリミティブ最大数
	LinePrimitive3D LineData[MaxLinePrimitives];

private: // 正四面体プリミティブ専用
	// 正四面体プリミティブ専用シェーダー
	ID3DBlob *Tetrahedron_vsBlob = nullptr; //正四面体プリミティブ用頂点シェーダー
	ID3DBlob *Tetrahedron_psBlob = nullptr; //正四面体プリミティブ用ピクセルシェーダー
	ID3DBlob *Tetrahedron_gsBlob = nullptr; //正四面体プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> TetrahedronRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> TetrahedronPipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> TetrahedronVertBuff;
	D3D12_VERTEX_BUFFER_VIEW TetrahedronvbView{};
	// 正四面体プリミティブ専用セットアップ
	void SetupTetrahedronPrimitive();
	// 正四面体プリミティブデータ
	static const int MaxTetrahedronPrimitives = 1; // 正四面体プリミティブ最大数
	TetrahedronPrimitive TetrahedronData[MaxTetrahedronPrimitives];

private: // 正八面体プリミティブ専用
	// 正八面体プリミティブ専用シェーダー
	ID3DBlob *Octahedron_vsBlob = nullptr; //正八面体プリミティブ用頂点シェーダー
	ID3DBlob *Octahedron_psBlob = nullptr; //正八面体プリミティブ用ピクセルシェーダー
	ID3DBlob *Octahedron_gsBlob = nullptr; //正八面体プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> OctahedronRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> OctahedronPipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> OctahedronVertBuff;
	D3D12_VERTEX_BUFFER_VIEW OctahedronvbView{};
	// 正八面体プリミティブ専用セットアップ
	void SetupOctahedronPrimitive();
	// 正八面体プリミティブデータ
	static const int MaxOctahedronPrimitives = 1; // 正八面体プリミティブ最大数
	OctahedronPrimitive OctahedronData[MaxOctahedronPrimitives];

private: //	正二十面体プリミティブ専用
	// 正二十面体プリミティブ専用シェーダー
	ID3DBlob *Icosahedron_vsBlob = nullptr; //正二十面体プリミティブ用頂点シェーダー
	ID3DBlob *Icosahedron_psBlob = nullptr; //正二十面体プリミティブ用ピクセルシェーダー
	ID3DBlob *Icosahedron_gsBlob = nullptr; //正二十面体プリミティブ用ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> IcosahedronRootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> IcosahedronPipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> IcosahedronVertBuff;
	D3D12_VERTEX_BUFFER_VIEW IcosahedronvbView{};
	// 正二十面体プリミティブ専用セットアップ
	void SetupIcosahedronPrimitive();
	// 正二十面体プリミティブデータ
	static const int MaxIcosahedronPrimitives = 1; // 正二十面体プリミティブ最大数
	IcosahedronPrimitive3D IcosahedronData[MaxIcosahedronPrimitives];

public:
	void Initialize();
	void Update();
	void CubeDrawAll();
	void LineDrawAll();
	void DrawTetrahedronAll();
	void DrawOctahedronAll();
	void DrawIcosahedronAll();
	void Draw();

	//void DrawCube(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color, bool Lighting = false);
	void DrawCube(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting = false);
	void DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color);
	void DrawTetrahedron(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4, XMFLOAT4 color, bool Lighting = false);
	void DrawOctahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false);
	void DrawIcosahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false);
	void DrawGrid(XMFLOAT3 pos1, XMFLOAT3 pos2, float interval,XMFLOAT4 color);

private:
	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
};