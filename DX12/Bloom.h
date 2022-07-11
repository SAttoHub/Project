#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

#include "PostGraphPrimitive.h"

class Bloom
{
private:
	struct ConstBufferData {
		XMMATRIX mat; //�ϊ��s��
	};
	ID3DBlob *errorBlob = nullptr;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> ConstBuff0;
private:
	// �摜�t�����v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Graph_vsBlob = nullptr; //�摜�t�����v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Graph_psBlob = nullptr; //�摜�t�����v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Graph_gsBlob = nullptr; //�摜�t�����v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> GraphRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> GraphPipelinestate;
	// ���_�o�b�t�@
	static const int MaxGraphPrimitives = 1; // �摜�t�����v���~�e�B�u�ő吔
	ComPtr<ID3D12Resource> GraphVertBuff[MaxGraphPrimitives];
	D3D12_VERTEX_BUFFER_VIEW GraphvbView[MaxGraphPrimitives]{};
	// �摜�t�����v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupGraphPrimitive();
	// �摜�t�����v���~�e�B�u�f�[�^
	PostGraphPrimitive GraphData[MaxGraphPrimitives];

	//�V�F�[�_�[�̓ǂݍ���
	void LoadShader(ID3DBlob **blob, LPCWSTR FileName, LPCSTR EntryPointName, LPCSTR ModelName);

private:
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	ComPtr<ID3D12Resource> TextureBuff;
public:
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	int TexNum;

	void Initialize();

	const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	void Draw(int TexNum1, int TexNum2, int TexNum3, int TexNum4);

	void ClearDepth();

	void PreDrawScene();
	void PostDrawScene();
};

