#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "TexManager.h"
#include"Input.h"
#include "PipelineManager.h"

struct SpriteVertex {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

//定数バッファ用構造体
struct SpriteConstBufferData {
	XMFLOAT4 color = {1,1,1,1}; //色(RGBA)
	XMMATRIX mat; //3D変換行列
};

struct SpriteData {
	ComPtr<ID3D12Resource> vertBuff; //頂点バッファ
	ComPtr<ID3D12Resource> constBuffB0; //定数バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{}; //頂点バッファビュー
	float spriteRotation = 0.0f; //Z軸回りの回転角
	XMVECTOR spritePosition = { 0,0,0,0 }; //座標
	XMMATRIX spriteMatWorld; //ワールド行列
	XMMATRIX spriteMatProjection; //射影行列
	SpriteConstBufferData constBufferData; //
	UINT texNumber; //テクスチャ番号
	bool AdjustSizeTexture = false;
	bool ResFlag = false;

	int ShaderNumber = 0;

	//スプライトのXY
	XMFLOAT2 Size = { 0,0 };
	//スプライトのUV
	XMFLOAT2 UV[4] = {};
	// テクスチャ左上座標
	XMFLOAT2 texLeftTop = { 0, 0 };
	// テクスチャの切り出しサイズ
	XMFLOAT2 texSize = { 100.0f, 100.0f };
};

class Sprite {
	static unsigned short indices[6];
	static D3D12_INDEX_BUFFER_VIEW ibView;
	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト
public:
	//初期化処理まとめ
	void DxSpriteBaceReset(Window *Win);

	//スプライト関連
	static void SpriteInit(SpriteData *sprdata);
	static void SpriteDraw(SpriteData *sprdata);
	static void spriteChangeSize(SpriteData *sprdata, float width, float height);
	static void spriteChangePos(SpriteData *sprdata, float X, float Y);
	static void spriteChangePosRefCenter(SpriteData *sprdata, float X, float Y);
	static void spriteChangeRotZ(SpriteData *sprdata, float RotZ);
	static void spriteChangeRect(SpriteData *sprdata, XMFLOAT2 texLeftTop, XMFLOAT2 texSize);



	static SpriteData SpriteCreate(int LT_X, int LT_Y, UINT texNum, UINT ShaderNum);

	static SpriteData ExtendSpriteCreate(int LT_X, int LT_Y, int RB_X, int RB_Y, UINT texNum, UINT ShaderNum);
};
