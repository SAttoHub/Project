#pragma once
#include "Model.h"
#include "..\..\CollisionInfo.h"
#include "..\Light\LightGroup.h"
#include "ModelManager.h"
#include "..\..\Engine\Common\Containers\MyFList.h"

class BaseCollider;

//�萔�o�b�t�@�p�\����
struct ConstBufferData {
	XMFLOAT4 color; //�F(RGBA)
	XMMATRIX mat; //3D�ϊ��s��
};

struct ConstBufferDataB0 {
	XMMATRIX view;
	XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
	XMMATRIX viewproj2; //�r���[�v���W�F�N�V�����s��2
	XMMATRIX world; //���[���h�s��
	XMFLOAT3 cameraPos; //�J�������W�i���[���h���W�j
	float pad1; // �p�f�B���O
	XMFLOAT4 color;
};

struct ConstBufferDataB0Inst {
	XMMATRIX world; //���[���h�s��
	XMFLOAT4 color;
	XMFLOAT4 uv;
	bool DrawFlag = true;
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient; // �A���r�G���g�W��
	float pad1; // �p�f�B���O
	XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
	float pad2; // �p�f�B���O
	XMFLOAT3 specular; // �X�y�L�����[�W��
	float alpha;	// �A���t�@
};

//�萔�o�b�t�@�p�\����
struct ConstBufferDataOther {
	UINT time; //�F(RGBA)
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

struct ConstBufferDataInst {
	XMMATRIX view;
	XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
	XMMATRIX viewproj2; //�r���[�v���W�F�N�V�����s��2
	XMFLOAT3 cameraPos; //�J�������W�i���[���h���W�j
};

struct ConstBufferDataShadow {
	XMMATRIX Light_view; // �r���[�s��
	XMMATRIX Light_viewproj;
	XMFLOAT3 Light_Pos;
};

//3D�I�u�W�F�N�g�^
struct Object3d {
	ComPtr<ID3D12Resource> constBuffB0;
	//�A�t�B���ϊ����
	XMFLOAT4 color = { 1,1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 oldrotation = { -1,0,0 };
	XMFLOAT3 oldscale = { -1,1,1 };
	XMFLOAT3 oldposition = { -1,0,0 };
	XMMATRIX matWorld;
	Object3d *parent = nullptr;
	UINT shaderNumber = 0;
	bool isBillboard = false;//�r���{�[�h
	bool DrawFlag = true; //DrawFlag
	bool alwaysUpdate = false; // ��Ƀ}�b�s���O��������

	bool UseShadow = false;
	bool UseDOF = false;
	bool UseBloom = false;


	vector<Vertex> vertices;
	vector<unsigned short> indices;
	Material material;
	int _indices = 0; //���_��
	bool isMaterial = false; //�}�e���A����K�����邩
	ComPtr<ID3D12Resource> vertBuff; //���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{}; //���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> indexBuff;

	//FBX�V�[��
	//FbxScene *fbxScene;
	Model *model = nullptr; //���f�����
	//FbxScene *fbxScene = nullptr;
	//std::vector<Model::Bone> bones;

public: //�X�L�j���O
	//1���b�V�������Ă�{�[���̍ő吔
	static const int MAX_BONES = 61;
	//�萔�o�b�t�@�p�\����(�X�L�j���O)
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�������x�{��
	float flameSpeed = 1.0f;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ������ǂ���
	bool isPlay = false;
	//�A�j���[�V���������[�v�����邩�ǂ���
	bool isLoop = false;
	//�A�j���[�V�����I�����ɂ����Ŏ~�߂邩�ǂ���
	bool isEndStop = false;
	//�Đ����̃A�j���[�V�����ԍ�
	int PlayAnimationNumber = 0;
	/// <summary>
	/// �A�j���[�V�����J�n(�ŏ�����)
	/// </summary>
	/// <param name="AnimationNumber">�Đ�����A�j���[�V�����̔ԍ� (�����͂� 0�� : -1 �Ō��݃Z�b�g���̃A�j���[�V���� )</param>
	/// <param name="Loop">���[�v�����邩�ǂ��� (�����͂� false )</param>
	void PlayAnimation(int AnimationNumber = 0, bool Loop = false);
	/// <summary>
	/// �A�j���[�V�����̈ꎞ��~
	/// </summary>
	void StopAnimation();
	/// <summary>
	/// �A�j���[�V�����̍Đ�(�r������)�@���A�j���[�V�������Z�b�g����Ă��Ȃ��ꍇ�͗�O�ŗ�����(����)
	/// </summary>
	void StartAnimation();
	/// <summary>
	/// �A�j���[�V�����̃��Z�b�g
	/// </summary>
	void ResetAnimetion();
	/// <summary>
	/// �A�j���[�V�������x��ύX
	/// </summary>
	/// <param name="Speed">���x</param>
	void ChangeAnimationSpeed(float Speed);

public:
	BaseCollider *collider = nullptr;
	Object3d *ptr = this;
	Object3d() = default;
	virtual ~Object3d();
	void SetCollider(BaseCollider *collider);
	void ColliderUpdate();
	virtual void OnCollision(const CollisionInfo &info) {}

public:
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX &GetMatWorld() { return matWorld; }

	void DeleteFunc();

	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffTime;
	UINT Timer = 0;

	ComPtr<ID3D12Resource> constBuffShadow;
};

struct InstanceObject {
	//�A�t�B���ϊ����
	XMFLOAT4 color = { 1,1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 oldrotation = { -1,0,0 };
	XMFLOAT3 oldscale = { -1,1,1 };
	XMFLOAT3 oldposition = { -1,0,0 };
	XMFLOAT2 LT_UV = { 0.0f,0.0f };
	XMFLOAT2 RB_UV = { 1.0f,1.0f };
	XMMATRIX matWorld;
	bool isDraw = true;
};

struct InstanceObjectsData {
	Model* model = nullptr; //���f�����

	vector<Vertex> vertices;
	vector<unsigned short> indices;
	Material material;
	int _indices = 0; //���_��
	bool isMaterial = false; //�}�e���A����K�����邩
	ComPtr<ID3D12Resource> vertBuff; //���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{}; //���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> indexBuff;

	UINT shaderNumber = 0;
	bool isBillboard = false;//�r���{�[�h
	bool DrawFlag = true; //DrawFlag
	bool alwaysUpdate = false; // ��Ƀ}�b�s���O��������
	bool UseShadow = false;
	bool UseDOF = false;
	bool UseBloom = false;

	static const int INSTANCE_MAX = 500;

	//1���b�V�������Ă�{�[���̍ő吔
	static const int MAX_BONES = 61;
	//�萔�o�b�t�@�p�\����(�X�L�j���O)
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};
	
	int InstanceCount;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�������x�{��
	float flameSpeed = 1.0f;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ������ǂ���
	bool isPlay = false;
	//�A�j���[�V���������[�v�����邩�ǂ���
	bool isLoop = false;
	//�A�j���[�V�����I�����ɂ����Ŏ~�߂邩�ǂ���
	bool isEndStop = false;
	//�Đ����̃A�j���[�V�����ԍ�
	int PlayAnimationNumber = 0;
	//void PlayAnimation(int AnimationNumber = 0, bool Loop = false);
	//void StopAnimation();
	//void StartAnimation();
	//void ResetAnimetion();
	//void ChangeAnimationSpeed(float Speed);

	std::vector<InstanceObject> object;

	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffSkin;
	ComPtr<ID3D12Resource> constBuffTime;
	ComPtr<ID3D12Resource> constBuffB4;
	ComPtr<ID3D12Resource> constBuffShadow;
	UINT Timer = 0;
};



class DirectX3dObject {
private:
	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g

	//�萔�o�b�t�@�r���[�̐���
	const int constantBufferNum = 1024; //�萔�o�b�t�@�̍ő吔

	//�萔�o�b�t�@�ւ̃f�[�^�̓]��
	void TransConstBuffer();
public:
	static int OldShaderLoad;//�O��Z�b�g�����V�F�[�_�[
	static bool isUpdateOther;
	static bool isUpdateOtherFlag;

	//����������
	void DirectX3DObjectReset(Window *Win);
	static void CreateInstancePipiline(ShaderManager* _shader);
	//�`��R�}���h
	void DrawReset() {
		for (int i = 0; i < (int)object3ds.size(); i++) {
			//[i].DrawFlag = false;
			object3ds[i]->DrawFlag = false;
			//forward_list_GetPtr(&object3ds, i)->DrawFlag = false;
		};
	}

	void Draw();


	static My_F_List<Object3d> object3ds;
	static My_F_List<InstanceObjectsData> InstanceObject3ds;

	DirectX3dObject() {
		//object3ds.reserve(constantBufferNum); //�萔�o�b�t�@�̍ő吔��������Ƀ��������m�ۂ��Ă���
	}

	static void UnionObject3d(Object3d *oya, Object3d *kodomo) {
		kodomo->parent = oya;
	};

	static void UpdateObject3d(Object3d *object, XMMATRIX &matView, XMMATRIX &matProjection);
	static void UpdateInstanceObject3d(InstanceObjectsData* object, XMMATRIX& matView, XMMATRIX& matProjection);

	/// <summary>
	/// �I�u�W�F�N�g�̍쐬
	/// </summary>
	/// <param name="model">�g�p���郂�f���f�[�^</param>
	/// <param name="pos">�����ʒu</param>
	/// <param name="ShaderNum">�g�p����V�F�[�_�[</param>
	/// <returns></returns>
	static Object3d *CreateObject(Model *model, XMFLOAT3 pos, UINT ShaderNum);
	static InstanceObjectsData* CreateInstanceObject(Model* model, XMFLOAT3 pos, UINT ShaderNum, int InstanceCount);



	static void AllObjectUpdate();

	static void DeleteObject(Object3d *object);
	static void DeleteObject(InstanceObjectsData* object);

	// ���C�g
	static LightGroup *lightGroup;
	/// <summary>
	/// ���C�g�O���[�v�̃Z�b�g
	/// </summary>
	/// <param name="lightGroup">���C�g�O���[�v</param>
	static void SetLightGroup(LightGroup *lightGroup) {
		DirectX3dObject::lightGroup = lightGroup;
	}

	//void DrawCube3D();
	static void ShadowDraw();
	static void DepthDraw();
	static void BloomDraw();
	static void BloomDepthDraw();
	static void DOFDraw();
};



//�I�u�W�F�N�g����������
static UINT RoundupConstantBufferSize(UINT size)
{
	size = (size + 255) & ~255;
	return size;
}
static ComPtr<ID3D12Resource> CreateBufferResource(D3D12_HEAP_TYPE type, UINT bufferSize, D3D12_RESOURCE_STATES state)
{
	ComPtr<ID3D12Resource> ret;
	HRESULT hr;

	const auto heapProps = CD3DX12_HEAP_PROPERTIES(type);
	const auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	hr = DirectXBase::dev->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		state,
		nullptr,
		IID_PPV_ARGS(&ret)
	);
	return ret;
}
void InitalizeObject3d(Object3d *object);
void InitalizeInstanceObject3d(InstanceObjectsData* object);

/// <summary>
/// �I�u�W�F�N�g�̕`��
/// </summary>
/// <param name="object">�g�p����I�u�W�F�N�g�f�[�^</param>
/// <returns></returns>
void Drawobject3d(Object3d *object);
void DepthDrawobject3d(Object3d *object);
void ShadowDepthDrawobject3d(Object3d *object);
void DOFDepthDrawobject3d(Object3d *object);

void Drawobject3d(InstanceObjectsData* object);
void DepthDrawobject3d(InstanceObjectsData* object);
void ShadowDepthDrawobject3d(InstanceObjectsData* object);
void DOFDepthDrawobject3d(InstanceObjectsData* object);
//void DepthDrawobject3d(InstanceObjectsData* object);
//void ShadowDepthDrawobject3d(InstanceObjectsData* object);
//void DOFDepthDrawobject3d(InstanceObjectsData* object);

/// <summary>
/// �I�u�W�F�N�g�̕`��
/// �f�t�H���g���C�g���g�p
/// </summary>
/// <param name="object">�g�p����I�u�W�F�N�g�f�[�^</param>
/// <returns></returns>
void Drawobject3dOfDefaultLight(Object3d *object);