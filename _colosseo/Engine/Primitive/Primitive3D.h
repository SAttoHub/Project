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
		//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
		result = D3DCompileFromFile(
			FileName, //�V�F�[�_�[�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
			EntryPointName, ModelName, //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
			0,
			*(&blob), &errorBlob
		);

		//�V�F�[�_�[�̃G���[���e��\��
		if (FAILED(result)) {
			//errorBlob����G���[���e��string�^�ɃR�s�[
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			//�G���[���e���o�̓E�B���h�E�ɕ\��
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
	}
}

// �v���~�e�B�u��b
class PrimitiveData {
public:
	// �v���~�e�B�u�V�F�[�_�[
	ID3DBlob* vsBlob = nullptr; //���_�V�F�[�_�[
	ID3DBlob* psBlob = nullptr; //�s�N�Z���V�F�[�_�[
	ID3DBlob* gsBlob = nullptr; //�W�I���g���V�F�[�_�[
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> Rootsignature;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> Pipelinestate;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> VertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
};

// ���v���~�e�B�u
class CubePrimData : public PrimitiveData {
public:
	// ���v���~�e�B�u�f�[�^
	static const int MaxPrimitives = 1; // ���v���~�e�B�u�ő吔
	CubePrimitive3D Data[MaxPrimitives];
	// ���v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupCubePrimitive();
};
// ���v���~�e�B�u
class LinePrimData : public PrimitiveData {
public:
	// ���v���~�e�B�u�f�[�^
	static const int MaxPrimitives = 1024; // ���v���~�e�B�u�ő吔
	LinePrimitive3D Data[MaxPrimitives];
	// ���v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupLinePrimitive();
};
// ���l�ʑ̃v���~�e�B�u
class TetrahedronPrimData : public PrimitiveData {
public:
	// ���l�ʑ̃v���~�e�B�u�f�[�^
	static const int MaxPrimitives = 1; // ���l�ʑ̃v���~�e�B�u�ő吔
	TetrahedronPrimitive Data[MaxPrimitives];
	// ���l�ʑ̃v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupTetrahedronPrimitive();
};
// �����ʑ̃v���~�e�B�u
class OctahedronPrimData : public PrimitiveData {
public:
	// �����ʑ̃v���~�e�B�u�f�[�^
	static const int MaxPrimitives = 1; // �����ʑ̃v���~�e�B�u�ő吔
	OctahedronPrimitive Data[MaxPrimitives];
	// �����ʑ̃v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupOctahedronPrimitive();
};
// ����\�ʑ̃v���~�e�B�u
class IcosahedronPrimData : public PrimitiveData {
public:
	// ����\�ʑ̃v���~�e�B�u�f�[�^
	static const int MaxPrimitives = 1; // ����\�ʑ̃v���~�e�B�u�ő吔
	IcosahedronPrimitive3D Data[MaxPrimitives];
	// ����\�ʑ̃v���~�e�B�u��p�Z�b�g�A�b�v
	void SetupIcosahedronPrimitive();
};

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

private: // ���v���~�e�B�u
	const bool CubeUse = false;
	std::unique_ptr<CubePrimData> CubePrim = nullptr;

private: // Line�v���~�e�B�u
	const bool LineUse = true;
	std::unique_ptr<LinePrimData> LinePrim = nullptr;
	int LineTexture;

private: // ���l�ʑ̃v���~�e�B�u
	const bool TetraUse = false;
	std::unique_ptr<TetrahedronPrimData> TetraPrim = nullptr;

private: // �����ʑ̃v���~�e�B�u
	const bool OctaUse = false;
	std::unique_ptr<OctahedronPrimData> OctaPrim = nullptr;

private: //	����\�ʑ̃v���~�e�B�u
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