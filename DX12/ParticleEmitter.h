#pragma once
#include "ParticleManager.h"

enum EmitType {
	PointEmit,
	RandPointEmit,
	RandPointEmit2,
	E_CIRCULAR,
	RandE_CIRCULAR
};

class pEmitter {
	XMFLOAT3 Velocity = XMFLOAT3{ 0,0,0 };
	XMFLOAT3 Accel;
	float StartScale;
	float EndScale;
	XMFLOAT4 StartColor;
	XMFLOAT4 EndColor;
	int LifeTime;
	int TextureNumber;
	UINT ShaderNumber;

	float Rand_width = 0;
	//RandPointEmit2
	XMFLOAT3 VelRandMIN;
	XMFLOAT3 AccRandMIN;
	XMFLOAT3 VelRandMAX;
	XMFLOAT3 AccRandMAX;

	//CIRCULAR�p
	XMFLOAT3 VelRandRange;
	XMFLOAT3 AccRandRange;
	float Radius;
	float RadiusRandRange;
	int RotaFlame;
	int RotaFlameRandRange;
public:
	pEmitter() {
		Accel = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
		StartScale = 0.0f;
		EndScale = 0.0f;
		StartColor = XMFLOAT4{ 0.0f,0.0f ,0.0f ,0.0f };
		EndColor = XMFLOAT4{ 0.0f,0.0f ,0.0f ,0.0f };
		LifeTime = 0;
		TextureNumber = 0;
		ShaderNumber = Particle;
		pos = XMFLOAT3{0.0f, 0.0f, 0.0f}; //�G�~�b�^�̊�_���W
		EmitQuantity = 0; //1�t���[���ɔ����������
		Type = RandPointEmit; //�G�~�b�^�[�̃^�C�v
		EmitLiveTime = 0;
	}

	XMFLOAT3 pos; //�G�~�b�^�̊�_���W
	int EmitQuantity; //1�t���[���ɔ����������
	int Type; //�G�~�b�^�[�̃^�C�v
	int EmitLiveTime;

	/// <summary>
	/// �G�~�b�^�̃X�e�[�^�X�����߂�
	/// </summary>
	/// <param name="EmitPos">�����̊�_</param>
	/// <param name="EmitQuantity">1�t���[���ɔ����������</param>
	/// <param name="EmitLiveTime">�G�~�b�^�[�̐�������(-1�Ŗ���)</param>
	/// <param name="Velocity">���̏���</param>
	/// <param name="Accel">���̉����x</param>
	/// <param name="StartScale">���̏����X�P�[��</param>
	/// <param name="EndScale">���̏I���X�P�[��</param>
	/// <param name="StartColor">���̏����F</param>
	/// <param name="EndColor">���̏I���F</param>
	/// <param name="LifeTime">���̐�������</param>
	/// <param name="TextureNumber">���ɑΉ�����e�N�X�`��</param>
	/// <param name="ShaderNumber">���ɑΉ�����V�F�[�_�[</param>
	void Create(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime, 
		XMFLOAT3 Velocity,
		XMFLOAT3 Accel,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle, 
		int Type = PointEmit);

	void CreateRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		float Rand_width,
		XMFLOAT3 Accel,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = RandPointEmit);

	void CreateRand2(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		XMFLOAT3 VelRandMIN,
		XMFLOAT3 VelRandMAX,
		XMFLOAT3 AccRandMIN,
		XMFLOAT3 AccRandMAX,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = RandPointEmit2);

	void CreateCIRCULAR(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		XMFLOAT3 Velocity,
		XMFLOAT3 Accel,
		float Radius,
		int RotaFlame,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = E_CIRCULAR);

	void CreateCIRCULARRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		XMFLOAT3 Velocity,
		XMFLOAT3 VelRandRange,
		XMFLOAT3 Accel,
		XMFLOAT3 AccRandRange,
		float Radius,
		float RadiusRandRange,
		int RotaFlame,
		int RotaFlameRandRange,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = RandE_CIRCULAR);

	void Emit(ParticleManager *P);

	int EmitTime = 1;
	int EmitCount = 0;
	void SetEmitTime(int i) {
		EmitTime = i;
	}
};

class ParticleEmitter {
	static const int EmittersLimit = 1024;
	static vector<pEmitter> Emitters;
public:
	static ParticleManager *Manager;
	ParticleEmitter() {
		Emitters.reserve(EmittersLimit);
	}
	static pEmitter *CreateEmitter(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		XMFLOAT3 Velocity,
		XMFLOAT3 Accel,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle);
	static pEmitter *CreateRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		float Rand_width,
		XMFLOAT3 Accel,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = RandPointEmit);
	static pEmitter *CreateRand2(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		XMFLOAT3 VelRandMIN,
		XMFLOAT3 VelRandMAX,
		XMFLOAT3 AccRandMIN,
		XMFLOAT3 AccRandMAX,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = RandPointEmit2);
	static pEmitter *CreateCIRCULAR(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		XMFLOAT3 Velocity,
		XMFLOAT3 Accel,
		float Radius,
		int RotaFlame,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = E_CIRCULAR);

	/// <summary>
	/// �����_���l�̂���~�^��������p�[�e�B�N���𐶐�����G�~�b�^�[�̐���
	/// </summary>
	/// <param name="EmitPos">�����ʒu</param>
	/// <param name="EmitQuantity">1�t���[���ɔ����������</param>
	/// <param name="EmitLiveTime">�G�~�b�^�[�̐�������(-1�Ŗ���)</param>
	/// <param name="Velocity">���̏����̍Œ�l</param>
	/// <param name="VelRandRange">���̏����ɉ��Z����l�̍ő�l</param>
	/// <param name="Accel">���̉����x�̍Œ�l</param>
	/// <param name="AccRandRange">���̉����x�ɉ��Z����l�̍ő�l</param>
	/// <param name="Radius">���a�̍Œ�l</param>
	/// <param name="RadiusRandRange">���a�ɉ��Z����l�̍ő�l</param>
	/// <param name="RotaFlame">���]����̂ɂ�����t���[�����̍Œ�l</param>
	/// <param name="RotaFlameRandRange">���]����̂ɂ�����t���[�����ɉ��Z����l�̍ő�l</param>
	/// <param name="StartScale">���̐������X�P�[��</param>
	/// <param name="EndScale">���̏I�����X�P�[��</param>
	/// <param name="StartColor">���̐������F</param>
	/// <param name="EndColor">���̏I���F</param>
	/// <param name="LifeTime">���̐�������</param>
	/// <param name="TextureNumber">�e�N�X�`��</param>
	/// <param name="ShaderNumber">�V�F�[�_�[</param>
	/// <param name="Type">�����Ō��߂�̂œ��͂��Ȃ��Ă������ł�</param>
	/// <returns></returns>
	static pEmitter *CreateCIRCULARRand(XMFLOAT3 EmitPos, int EmitQuantity, int EmitLiveTime,
		XMFLOAT3 Velocity,
		XMFLOAT3 VelRandRange,
		XMFLOAT3 Accel,
		XMFLOAT3 AccRandRange,
		float Radius,
		float RadiusRandRange,
		int RotaFlame,
		int RotaFlameRandRange,
		float StartScale,
		float EndScale,
		XMFLOAT4 StartColor,
		XMFLOAT4 EndColor,
		int LifeTime,
		int TextureNumber,
		UINT ShaderNumber = Particle,
		int Type = RandE_CIRCULAR);
	static void update();
	static void Draw();
};

