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

//�萔�o�b�t�@�p�\����
struct SpriteConstBufferData {
	XMFLOAT4 color = {1,1,1,1}; //�F(RGBA)
	XMMATRIX mat; //3D�ϊ��s��
};

struct SpriteData {
	ComPtr<ID3D12Resource> vertBuff; //���_�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0; //�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{}; //���_�o�b�t�@�r���[
	float spriteRotation = 0.0f; //Z�����̉�]�p
	XMVECTOR spritePosition = { 0,0,0,0 }; //���W
	XMMATRIX spriteMatWorld; //���[���h�s��
	XMMATRIX spriteMatProjection; //�ˉe�s��
	SpriteConstBufferData constBufferData; //
	UINT texNumber; //�e�N�X�`���ԍ�
	bool AdjustSizeTexture = false;
	bool ResFlag = false;

	int ShaderNumber = 0;

	//�X�v���C�g��XY
	XMFLOAT2 Size = { 0,0 };
	//�X�v���C�g��UV
	XMFLOAT2 UV[4] = {};
	// �e�N�X�`��������W
	XMFLOAT2 texLeftTop = { 0, 0 };
	// �e�N�X�`���̐؂�o���T�C�Y
	XMFLOAT2 texSize = { 100.0f, 100.0f };
};

class Sprite {
	static unsigned short indices[6];
	static D3D12_INDEX_BUFFER_VIEW ibView;
	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g
public:
	//�����������܂Ƃ�
	void DxSpriteBaceReset(Window *Win);

	//�X�v���C�g�֘A
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
