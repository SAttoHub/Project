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
#include "Smath.h"
using namespace Smath;
#include "ColorFuncs.h"


struct ParticleConstBufferData {
	XMMATRIX mat; //3D�ϊ��s��
	XMMATRIX matBillboard;
};

struct ParticleConstBufferDataB1
{
	XMFLOAT3 ambient; // �A���r�G���g�W��
	float pad1; // �p�f�B���O
	XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
	float pad2; // �p�f�B���O
	XMFLOAT3 specular; // �X�y�L�����[�W��
	float alpha;	// �A���t�@
};

//���_�f�[�^�\����
struct ParticleVertexPos {
	XMFLOAT3 pos;
	float scale;
	XMFLOAT4 color = { 1,1,1,1 };
};

enum ParticleType {
	P_Normal,
	P_CIRCULAR
};

class pEmitter;

//�p�[�e�B�N���̗�
struct SingleParticle {
	XMFLOAT3 position = {};
	XMFLOAT3 velocity = {};
	XMFLOAT3 accel = {};
	float scale = 1.0f;
	float start_scale = 1.0f;
	float end_scale = 1.0f;
	float rotation = 0.0f;
	int now_frame = 0;
	int end_frame = 0;

	XMFLOAT4 color = { 1,1,1,1 };
	XMFLOAT4 startColor = { 1,1,0,1 };
	XMFLOAT4 endColor = { 1,0,1,1 };

	int Type = P_Normal;
	float Radius = 0;
	XMFLOAT3 Center = {};
	int RotaFlame = 0; //����Ɋ|����t���[����

	pEmitter *emitter; //�����𐶂ݏo�����G�~�b�^�̃|�C���^
};

//�p�[�e�B�N������
struct ParticleObj {
	std::string tag = "None";

	UINT texNumber = 0; //�e�N�X�`���ԍ�
	UINT shaderNumber = 0; //�V�F�[�_�[�ԍ�
	ParticleObj() {
		State.reserve(MaxParticle);
	}
	~ParticleObj() {
	}
	vector<SingleParticle> State; //�X�e�[�^�X

	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> vertBuff; //���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{}; //���_�o�b�t�@�r���[
	int MaxParticle = 2048;
};

struct ParticleMaterial {
	std::string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float alpha;
	std::string textureFilename;
	ParticleMaterial() {
		ambient = { 0.0f, 0.0f, 0.0f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}
};




class ParticleManager {
private:
	ParticleMaterial material;

	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g



	//�萔�o�b�t�@�r���[�̐���
	const int constantBufferNum = 128; //�萔�o�b�t�@�̍ő吔
	void CreateConstBufferView();

	//�萔�o�b�t�@�ւ̃f�[�^�̓]��
	void TransConstBuffer();
public:
	//����������
	void DirectX3DObjectReset(Window *Win);
	//�`��R�}���h
	void Draw();
	void SetBlend(int Blend_Num);


	//�ˉe�ϊ��p



	std::vector<ParticleObj> object3ds;

	ParticleManager() {
		object3ds.reserve(constantBufferNum); //�萔�o�b�t�@�̍ő吔��������Ƀ��������m�ۂ��Ă���
	}

	void UpdateObject3d(XMMATRIX &matView, XMMATRIX &matProjection);

	int OldShaderLoad = -1;

	void Create(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, UINT texNum, UINT ShaderNum, XMFLOAT4 startColor = { 1,1,1,1 },
		XMFLOAT4 endColor = { 1,1,1,1 });
	
	void CreateRand(std::string tag,
		int count,
		XMFLOAT3 pos,
		float Rand_width,
		XMFLOAT3 acc,
		int lifetime,
		float start_scale,
		float end_scale,
		UINT texNum,
		UINT ShaderNum, 
		XMFLOAT4 startColor = { 1,1,1,1 },
		XMFLOAT4 endColor = { 1,1,1,1 });

	void ParDrawobject3d(ParticleObj *object);



	int DrawCount = 0;

	// R = ���a
	void earthquakeCreate(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float R, UINT texNum, UINT ShaderNum);
	
	void earthquakeCreate2(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float RX, float RZ, UINT texNum, UINT ShaderNum);

	void earthquakeCreate3(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float RX, float RZ, float Rot, UINT texNum, UINT ShaderNum);
	
	void CreateRandVel(std::string tag, XMFLOAT3 pos, XMFLOAT3 velMIN, XMFLOAT3 velMAX, XMFLOAT3 accMIN, XMFLOAT3 accMAX, int lifetime, float start_scale, float end_scale, int count, UINT texNum, UINT ShaderNum, XMFLOAT4 startColor = { 1,1,1,1 },
		XMFLOAT4 endColor = { 1,1,1,1 });


	//P_CIRCULAR�p�[�e�B�N��
	
	/// <summary>
	/// �~�^��������p�[�e�B�N���̐���
	/// </summary>
	/// <param name="tag">�^�O</param>
	/// <param name="count">��x�ɐ������闱�̐�</param>
	/// <param name="CenterPos">�ʒu</param>
	/// <param name="Radius">���a</param>
	/// <param name="RotaFlame">���]����̂ɂ�����t���[����</param>
	/// <param name="lifetime">��������</param>
	/// <param name="start_scale">�������X�P�[��</param>
	/// <param name="end_scale">�I�����X�P�[��</param>
	/// <param name="texNum">�e�N�X�`��</param>
	/// <param name="ShaderNum">�V�F�[�_�[</param>
	/// <param name="startColor">�������F</param>
	/// <param name="endColor">�I�����F</param>
	void CreateCIRCULARParticle(std::string tag,
		int count,
		XMFLOAT3 CenterPos,
		XMFLOAT3 vel, 
		XMFLOAT3 acc,
		float Radius,
		int RotaFlame,
		int lifetime,
		float start_scale,
		float end_scale,
		UINT texNum,
		pEmitter *emitter = nullptr,
		UINT ShaderNum = Particle,
		XMFLOAT4 startColor = { 1,1,1,1 },
		XMFLOAT4 endColor = { 1,1,1,1 });
};



//�I�u�W�F�N�g����������



