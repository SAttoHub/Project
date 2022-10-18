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
		XMMATRIX mat; //3D�ϊ��s��
		XMMATRIX matBillboard;
	};
	ComPtr<ID3D12Resource> ConstBuff0;

	ID3DBlob *errorBlob = nullptr;

private: // ���v���~�e�B�u��p
	// ���v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Cube_vsBlob = nullptr; //���v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Cube_psBlob = nullptr; //���v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Cube_gsBlob = nullptr; //���v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> CubeRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> CubePipelinestate;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> CubeVertBuff;
	D3D12_VERTEX_BUFFER_VIEW CubevbView{};
	// ���v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupCubePrimitive();
	// ���v���~�e�B�u�f�[�^
	static const int MaxCubePrimitives = 32; // ���v���~�e�B�u�ő吔
	CubePrimitive3D CubeData[MaxCubePrimitives];

private: // Line�v���~�e�B�u��p
	// ���v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Line_vsBlob = nullptr; //���v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Line_psBlob = nullptr; //���v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Line_gsBlob = nullptr; //���v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> LineRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> LinePipelinestate;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> LineVertBuff;
	D3D12_VERTEX_BUFFER_VIEW LinevbView{};
	// ���v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupLinePrimitive();
	// ���v���~�e�B�u�f�[�^
	static const int MaxLinePrimitives = 512; // ���v���~�e�B�u�ő吔
	LinePrimitive3D LineData[MaxLinePrimitives];

private: // ���l�ʑ̃v���~�e�B�u��p
	// ���l�ʑ̃v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Tetrahedron_vsBlob = nullptr; //���l�ʑ̃v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Tetrahedron_psBlob = nullptr; //���l�ʑ̃v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Tetrahedron_gsBlob = nullptr; //���l�ʑ̃v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> TetrahedronRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> TetrahedronPipelinestate;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> TetrahedronVertBuff;
	D3D12_VERTEX_BUFFER_VIEW TetrahedronvbView{};
	// ���l�ʑ̃v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupTetrahedronPrimitive();
	// ���l�ʑ̃v���~�e�B�u�f�[�^
	static const int MaxTetrahedronPrimitives = 1; // ���l�ʑ̃v���~�e�B�u�ő吔
	TetrahedronPrimitive TetrahedronData[MaxTetrahedronPrimitives];

private: // �����ʑ̃v���~�e�B�u��p
	// �����ʑ̃v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Octahedron_vsBlob = nullptr; //�����ʑ̃v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Octahedron_psBlob = nullptr; //�����ʑ̃v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Octahedron_gsBlob = nullptr; //�����ʑ̃v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> OctahedronRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> OctahedronPipelinestate;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> OctahedronVertBuff;
	D3D12_VERTEX_BUFFER_VIEW OctahedronvbView{};
	// �����ʑ̃v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupOctahedronPrimitive();
	// �����ʑ̃v���~�e�B�u�f�[�^
	static const int MaxOctahedronPrimitives = 1; // �����ʑ̃v���~�e�B�u�ő吔
	OctahedronPrimitive OctahedronData[MaxOctahedronPrimitives];

private: //	����\�ʑ̃v���~�e�B�u��p
	// ����\�ʑ̃v���~�e�B�u��p�V�F�[�_�[
	ID3DBlob *Icosahedron_vsBlob = nullptr; //����\�ʑ̃v���~�e�B�u�p���_�V�F�[�_�[
	ID3DBlob *Icosahedron_psBlob = nullptr; //����\�ʑ̃v���~�e�B�u�p�s�N�Z���V�F�[�_�[
	ID3DBlob *Icosahedron_gsBlob = nullptr; //����\�ʑ̃v���~�e�B�u�p�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> IcosahedronRootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> IcosahedronPipelinestate;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> IcosahedronVertBuff;
	D3D12_VERTEX_BUFFER_VIEW IcosahedronvbView{};
	// ����\�ʑ̃v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupIcosahedronPrimitive();
	// ����\�ʑ̃v���~�e�B�u�f�[�^
	static const int MaxIcosahedronPrimitives = 1; // ����\�ʑ̃v���~�e�B�u�ő吔
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