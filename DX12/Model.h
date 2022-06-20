#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
#include"Input.h"
#include "TexManager.h"
#include <string>
#include <list>
#include <array>
#include <memory>
#include "Camera.h"
#include "PipelineManager.h"
#include <fstream>
#include <sstream>
using namespace std;
#include "fbxsdk.h"
#include <unordered_map>

struct VertexPosUv {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

//���_�f�[�^�\����
struct VertexPos {
	XMFLOAT3 pos;
};

struct Material {
	std::string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float alpha;
	std::string textureFilename;

	Material() {
		ambient = { 1.0f, 1.0f, 1.0f };
		diffuse = { 1.0f, 1.0f, 1.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 0.3f;
	}

	UINT texNumber = 0; //�e�N�X�`���ԍ�
	UINT BumpTexNumber = 0; //�o���v�}�b�s���O�p�e�N�X�`���ԍ�
	UINT HeightTexNumber = 0; //�����}�b�s���O�p�e�N�X�`���ԍ�
	ComPtr<ID3D12Resource> constBuffB1;
};

struct FBXNode {
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	FBXNode *parent = nullptr;
};

//�{�[���C���f�b�N�X�̍ő吔
static const int MAX_BONE_INDICES = 4;
struct Vertex {
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 normal = { 0,0,0 }; //�@���x�N�g��
	XMFLOAT2 uv = { 0,0 };
	signed short boneIndex[MAX_BONE_INDICES] = { -1 };
	signed short boneIndexB[MAX_BONE_INDICES] = { -1 };
	float boneWeight[MAX_BONE_INDICES] = { 0 };
	float boneWeightB[MAX_BONE_INDICES] = { 0 };
};

struct Model {
	friend class FbxLoader;
public:
	string fileName; //�t�@�C����
	int modelID;

	Model *parent = nullptr;

public: //�萔

	std::string tag = "None";
	vector<Vertex> vertices;
	vector<unsigned short> indices;
	Material material;

	int _indices = 0; //���_��
	bool isMaterial = false; //�}�e���A����K�����邩
	XMFLOAT3 scale = { 1,1,1 };

	//FBX�p
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<FBXNode> nodes;
	//���b�V�������m�[�h
	FBXNode *meshNode = nullptr;
	//�`��
	void Draw();
	//���f���̕ό`�s��擾
	const XMMATRIX &GetModelTransform() { return meshNode->globalTransform; }

	//�{�[���\����
	struct Bone {
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[(FBX���̃{�[�����)
		FbxCluster *fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string &name) {
			this->name = name;
		}
	};
	//�{�[���z��
	std::vector<Bone> bones;
	//getter
	std::vector<Bone> &GetBones() { return bones; }
	//FBX�V�[��
	FbxScene *fbxScene = nullptr;
	//getter
	FbxScene *GetFbxScene() { return fbxScene; }
	~Model();

	//�X���[�W���O
	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	/// <summary>
	/// ���_�f�[�^�̐����擾
	/// </summary>
	/// <returns></returns>
	inline size_t GetVertexCount() { return vertices.size(); }
	/// <summary>
	/// �G�b�W�������f�[�^�̒ǉ�
	/// </summary>
	/// <param name="indexPosition">���W�C���f�b�N�X</param>
	/// <param name="indexVertex">���_�C���f�b�N�X</param>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);
	/// <summary>
	/// ���������ꂽ���_�@���̌v�Z
	/// </summary>
	void CalculateSmoothedVertexNormals();
};