#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

class ShaderManager {
public:

	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g

	ComPtr<ID3DBlob> FBX_vsBlob; //FBX���_�V�F�[�_�[
	ComPtr<ID3DBlob> FBX_psBlob; //FBX�s�N�Z���V�F�[�_�[
	ComPtr<ID3DBlob> FBX_Bump_psBlob; //FBX�s�N�Z���V�F�[�_�[
	ComPtr<ID3DBlob> FBX_height_psBlob; //FBX�s�N�Z���V�F�[�_�[
	ComPtr<ID3DBlob> FBX_OutLine_psBlob; // �A�E�g���C���V�F�[�_�[

	ComPtr<ID3DBlob> FBX_Line_gsBlob; // �ӂ̂�
	ComPtr<ID3DBlob> FBX_Line_psBlob; // �ӂ̂�

	ComPtr<ID3DBlob> FBX_Sea_psBlob; // �C���ۂ�

	ComPtr<ID3DBlob> FBX_Depth_psBlob; // �[�x�l�p

	ComPtr<ID3DBlob> FBX_DOF_psBlob; // �[�x�l�p

	ComPtr<ID3DBlob> FBX_Shadow_Depth_vsBlob; // �[�x�l�p
	ComPtr<ID3DBlob> FBX_Shadow_Depth_psBlob; // �[�x�l�p

	ShaderManager();
	~ShaderManager() {};

	void LoadShaderAll();

	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);
};

