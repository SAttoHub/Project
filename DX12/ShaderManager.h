#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

class ShaderManager {
public:

	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト

	ComPtr<ID3DBlob> FBX_vsBlob; //FBX頂点シェーダー
	ComPtr<ID3DBlob> FBX_psBlob; //FBXピクセルシェーダー
	ComPtr<ID3DBlob> FBX_Bump_psBlob; //FBXピクセルシェーダー
	ComPtr<ID3DBlob> FBX_height_psBlob; //FBXピクセルシェーダー
	ComPtr<ID3DBlob> FBX_OutLine_psBlob; // アウトラインシェーダー

	ComPtr<ID3DBlob> FBX_Line_gsBlob; // 辺のみ
	ComPtr<ID3DBlob> FBX_Line_psBlob; // 辺のみ

	ComPtr<ID3DBlob> FBX_Sea_psBlob; // 海っぽい

	ComPtr<ID3DBlob> FBX_Depth_psBlob; // 深度値用

	ComPtr<ID3DBlob> FBX_DOF_psBlob; // 深度値用

	ComPtr<ID3DBlob> FBX_Shadow_Depth_vsBlob; // 深度値用
	ComPtr<ID3DBlob> FBX_Shadow_Depth_psBlob; // 深度値用

	ShaderManager();
	~ShaderManager() {};

	void LoadShaderAll();

	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
};

