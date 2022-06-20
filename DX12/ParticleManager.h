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
	XMMATRIX mat; //3D変換行列
	XMMATRIX matBillboard;
};

struct ParticleConstBufferDataB1
{
	XMFLOAT3 ambient; // アンビエント係数
	float pad1; // パディング
	XMFLOAT3 diffuse; // ディフューズ係数
	float pad2; // パディング
	XMFLOAT3 specular; // スペキュラー係数
	float alpha;	// アルファ
};

//頂点データ構造体
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

//パーティクルの粒
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
	int RotaFlame = 0; //一周に掛かるフレーム数

	pEmitter *emitter; //自分を生み出したエミッタのポインタ
};

//パーティクル総合
struct ParticleObj {
	std::string tag = "None";

	UINT texNumber = 0; //テクスチャ番号
	UINT shaderNumber = 0; //シェーダー番号
	ParticleObj() {
		State.reserve(MaxParticle);
	}
	~ParticleObj() {
	}
	vector<SingleParticle> State; //ステータス

	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> vertBuff; //頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{}; //頂点バッファビュー
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

	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト



	//定数バッファビューの生成
	const int constantBufferNum = 128; //定数バッファの最大数
	void CreateConstBufferView();

	//定数バッファへのデータの転送
	void TransConstBuffer();
public:
	//初期化処理
	void DirectX3DObjectReset(Window *Win);
	//描画コマンド
	void Draw();
	void SetBlend(int Blend_Num);


	//射影変換用



	std::vector<ParticleObj> object3ds;

	ParticleManager() {
		object3ds.reserve(constantBufferNum); //定数バッファの最大数分だけ先にメモリを確保しておく
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

	// R = 半径
	void earthquakeCreate(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float R, UINT texNum, UINT ShaderNum);
	
	void earthquakeCreate2(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float RX, float RZ, UINT texNum, UINT ShaderNum);

	void earthquakeCreate3(std::string tag, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 acc, int lifetime, float start_scale, float end_scale, int count, float RX, float RZ, float Rot, UINT texNum, UINT ShaderNum);
	
	void CreateRandVel(std::string tag, XMFLOAT3 pos, XMFLOAT3 velMIN, XMFLOAT3 velMAX, XMFLOAT3 accMIN, XMFLOAT3 accMAX, int lifetime, float start_scale, float end_scale, int count, UINT texNum, UINT ShaderNum, XMFLOAT4 startColor = { 1,1,1,1 },
		XMFLOAT4 endColor = { 1,1,1,1 });


	//P_CIRCULARパーティクル
	
	/// <summary>
	/// 円運動をするパーティクルの生成
	/// </summary>
	/// <param name="tag">タグ</param>
	/// <param name="count">一度に生成する粒の数</param>
	/// <param name="CenterPos">位置</param>
	/// <param name="Radius">半径</param>
	/// <param name="RotaFlame">一回転するのにかかるフレーム数</param>
	/// <param name="lifetime">生存時間</param>
	/// <param name="start_scale">生成時スケール</param>
	/// <param name="end_scale">終了時スケール</param>
	/// <param name="texNum">テクスチャ</param>
	/// <param name="ShaderNum">シェーダー</param>
	/// <param name="startColor">生成時色</param>
	/// <param name="endColor">終了時色</param>
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



//オブジェクト初期化処理



