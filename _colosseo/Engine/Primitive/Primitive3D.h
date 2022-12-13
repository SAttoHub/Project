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

namespace PrimitiveLoadShader {
	static void LoadShader(ID3DBlob** blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName) {
		HRESULT result;
		ID3DBlob* errorBlob = nullptr;
		//シェーダーの読み込みとコンパイル
		result = D3DCompileFromFile(
			FileName, //シェーダーファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
			EntryPointName, ModelName, //エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
			0,
			*(&blob), &errorBlob
		);

		//シェーダーのエラー内容を表示
		if (FAILED(result)) {
			//errorBlobからエラー内容をstring型にコピー
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			//エラー内容を出力ウィンドウに表示
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
	}
}

// プリミティブ基礎
class PrimitiveData {
public:
	// プリミティブシェーダー
	ID3DBlob* vsBlob = nullptr; //頂点シェーダー
	ID3DBlob* psBlob = nullptr; //ピクセルシェーダー
	ID3DBlob* gsBlob = nullptr; //ジオメトリシェーダー
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> Rootsignature;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> Pipelinestate;
	// 頂点バッファ
	ComPtr<ID3D12Resource> VertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
};

// 箱プリミティブ
class CubePrimData : public PrimitiveData {
public:
	// 箱プリミティブデータ
	static const int MaxPrimitives = 1; // 箱プリミティブ最大数
	CubePrimitive3D Data[MaxPrimitives];
	// 箱プリミティブ専用セットアップ
	void SetupCubePrimitive();
};
// 線プリミティブ
class LinePrimData : public PrimitiveData {
public:
	// 線プリミティブデータ
	static const int MaxPrimitives = 1024; // 線プリミティブ最大数
	LinePrimitive3D Data[MaxPrimitives];
	// 線プリミティブ専用セットアップ
	void SetupLinePrimitive();
};
// 正四面体プリミティブ
class TetrahedronPrimData : public PrimitiveData {
public:
	// 正四面体プリミティブデータ
	static const int MaxPrimitives = 1; // 正四面体プリミティブ最大数
	TetrahedronPrimitive Data[MaxPrimitives];
	// 正四面体プリミティブ専用セットアップ
	void SetupTetrahedronPrimitive();
};
// 正八面体プリミティブ
class OctahedronPrimData : public PrimitiveData {
public:
	// 正八面体プリミティブデータ
	static const int MaxPrimitives = 1; // 正八面体プリミティブ最大数
	OctahedronPrimitive Data[MaxPrimitives];
	// 正八面体プリミティブ専用セットアップ
	void SetupOctahedronPrimitive();
};
// 正二十面体プリミティブ
class IcosahedronPrimData : public PrimitiveData {
public:
	// 正二十面体プリミティブデータ
	static const int MaxPrimitives = 1; // 正二十面体プリミティブ最大数
	IcosahedronPrimitive3D Data[MaxPrimitives];
	// 正二十面体プリミティブ専用セットアップ
	void SetupIcosahedronPrimitive();
};

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

private: // 箱プリミティブ
	const bool CubeUse = false;
	std::unique_ptr<CubePrimData> CubePrim = nullptr;

private: // Lineプリミティブ
	const bool LineUse = true;
	std::unique_ptr<LinePrimData> LinePrim = nullptr;
	int LineTexture;

private: // 正四面体プリミティブ
	const bool TetraUse = false;
	std::unique_ptr<TetrahedronPrimData> TetraPrim = nullptr;

private: // 正八面体プリミティブ
	const bool OctaUse = false;
	std::unique_ptr<OctahedronPrimData> OctaPrim = nullptr;

private: //	正二十面体プリミティブ
	const bool IcosaUse = false;
	std::unique_ptr<IcosahedronPrimData> IcosaPrim = nullptr;

public:
	void Initialize();
	void Update();
	void CubeDrawAll();
	void LineDrawAll();
	void DrawTetrahedronAll();
	void DrawOctahedronAll();
	void DrawIcosahedronAll();
	void Draw();

	void DrawCube(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 scale, XMFLOAT3 rotate, XMFLOAT4 color, bool Lighting = false);
	void DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color);
	void DrawTetrahedron(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4, XMFLOAT4 color, bool Lighting = false);
	void DrawOctahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false);
	void DrawIcosahedron(XMFLOAT3 pos, float Radius, XMFLOAT4 color, bool Lighting = false);
	void DrawGrid(XMFLOAT3 pos1, XMFLOAT3 pos2, float interval,XMFLOAT4 color);

private:
	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
};