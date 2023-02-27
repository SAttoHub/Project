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
		XMMATRIX mat; //�ϊ��s��
	};
	ComPtr<ID3D12Resource> ConstBuff0;

	ID3DBlob *errorBlob = nullptr;

private: // ���v���~�e�B�u��p
	// ���v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Box_vsBlob = nullptr; //���v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Box_psBlob = nullptr; //���v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Box_gsBlob = nullptr; //���v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> BoxRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> BoxPipelinestate;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> BoxVertBuff;
	D3D12_VERTEX_BUFFER_VIEW BoxvbView{};
	// ���v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupBoxPrimitive();
	// ���v���~�e�B�u�f�[�^
	static const int MaxBoxPrimitives = 512; // ���v���~�e�B�u�ő吔
	BoxPrimitive BoxData[MaxBoxPrimitives];

private: // �摜�t�����v���~�e�B�u��p
	// �摜�t�����v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Graph_vsBlob = nullptr; //�摜�t�����v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Graph_psBlob = nullptr; //�摜�t�����v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Graph_gsBlob = nullptr; //�摜�t�����v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> GraphRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> GraphPipelinestate;
	// ���_�o�b�t�@
	static const int MaxGraphPrimitives = 512; // �摜�t�����v���~�e�B�u�ő吔
	ComPtr<ID3D12Resource> GraphVertBuff[MaxGraphPrimitives];
	D3D12_VERTEX_BUFFER_VIEW GraphvbView[MaxGraphPrimitives]{};

	ComPtr<ID3D12Resource> BackGraphVertBuff[MaxGraphPrimitives];
	D3D12_VERTEX_BUFFER_VIEW BackGraphvbView[MaxGraphPrimitives]{};

	// �摜�t�����v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupGraphPrimitive();
	// �摜�t�����v���~�e�B�u�f�[�^
	GraphPrimitive GraphData[MaxGraphPrimitives];

//private: // ���v���~�e�B�u��p
//// ���v���~�e�B�u��p�V�F�[�_�[
//	ID3DBlob *Line_vsBlob = nullptr; //���v���~�e�B�u�p���_�V�F�[�_�[
//	ID3DBlob *Line_psBlob = nullptr; //���v���~�e�B�u�p�s�N�Z���V�F�[�_�[
//	ID3DBlob *Line_gsBlob = nullptr; //���v���~�e�B�u�p�W�I���g���V�F�[�_�[
//	// ���[�g�V�O�l�`��
//	ComPtr<ID3D12RootSignature> LineRootsignature;
//	// �p�C�v���C���X�e�[�g
//	ComPtr<ID3D12PipelineState> LinePipelinestate;
//	// ���_�o�b�t�@
//	ComPtr<ID3D12Resource> LineVertBuff;
//	D3D12_VERTEX_BUFFER_VIEW LinevbView{};
//	// ���v���~�e�B�u��p�Z�b�g�A�b�v
//	void SetupLinePrimitive();
//	// ���v���~�e�B�u�f�[�^
//	static const int MaxLinePrimitives = 128; // ���v���~�e�B�u�ő吔
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