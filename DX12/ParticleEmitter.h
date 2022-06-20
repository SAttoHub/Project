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

	//CIRCULAR用
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
		pos = XMFLOAT3{0.0f, 0.0f, 0.0f}; //エミッタの基点座標
		EmitQuantity = 0; //1フレームに発生させる個数
		Type = RandPointEmit; //エミッターのタイプ
		EmitLiveTime = 0;
	}

	XMFLOAT3 pos; //エミッタの基点座標
	int EmitQuantity; //1フレームに発生させる個数
	int Type; //エミッターのタイプ
	int EmitLiveTime;

	/// <summary>
	/// エミッタのステータスを決める
	/// </summary>
	/// <param name="EmitPos">発生の基準点</param>
	/// <param name="EmitQuantity">1フレームに発生させる個数</param>
	/// <param name="EmitLiveTime">エミッターの生存時間(-1で無限)</param>
	/// <param name="Velocity">粒の初速</param>
	/// <param name="Accel">粒の加速度</param>
	/// <param name="StartScale">粒の初期スケール</param>
	/// <param name="EndScale">粒の終了スケール</param>
	/// <param name="StartColor">粒の初期色</param>
	/// <param name="EndColor">粒の終了色</param>
	/// <param name="LifeTime">粒の生存時間</param>
	/// <param name="TextureNumber">粒に対応するテクスチャ</param>
	/// <param name="ShaderNumber">粒に対応するシェーダー</param>
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
	/// ランダム値のある円運動をするパーティクルを生成するエミッターの生成
	/// </summary>
	/// <param name="EmitPos">初期位置</param>
	/// <param name="EmitQuantity">1フレームに発生させる個数</param>
	/// <param name="EmitLiveTime">エミッターの生存時間(-1で無限)</param>
	/// <param name="Velocity">粒の初速の最低値</param>
	/// <param name="VelRandRange">粒の初速に加算する値の最大値</param>
	/// <param name="Accel">粒の加速度の最低値</param>
	/// <param name="AccRandRange">粒の加速度に加算する値の最大値</param>
	/// <param name="Radius">半径の最低値</param>
	/// <param name="RadiusRandRange">半径に加算する値の最大値</param>
	/// <param name="RotaFlame">一回転するのにかかるフレーム数の最低値</param>
	/// <param name="RotaFlameRandRange">一回転するのにかかるフレーム数に加算する値の最大値</param>
	/// <param name="StartScale">粒の生成時スケール</param>
	/// <param name="EndScale">粒の終了時スケール</param>
	/// <param name="StartColor">粒の生成時色</param>
	/// <param name="EndColor">粒の終了色</param>
	/// <param name="LifeTime">粒の生存時間</param>
	/// <param name="TextureNumber">テクスチャ</param>
	/// <param name="ShaderNumber">シェーダー</param>
	/// <param name="Type">自動で決めるので入力しなくてもいいです</param>
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

