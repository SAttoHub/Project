#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
#include <string>

#include "Engine\Common\Colors\ColorFuncs.h"

using namespace std;

class TexManager {
	static UINT TexNum;
public:
	struct Texture {
		int TexNum;
		ComPtr<ID3D12Resource> TextureBuff;
		TexMetadata metadata;
		ScratchImage scratchImg;
		const Image *img;
		XMFLOAT4 colorData = { -1,-1,-1,-1 };
		string FileName;
		XMFLOAT2 TextureSize;
		// �t�H���g�p
		char tag;
		GLYPHMETRICS gm;
		TEXTMETRIC tm;
		UINT Mcode; // �����R�[�h�ۑ�
	};
	TexManager();
	~TexManager();
	void TextureDescHeapCreate();//�f�X�N���v�^�q�[�v�̐������K��3Dobject��spriteBase�̏�������ɌĂяo��
	static const int TextureSRVCount; //�e�N�X�`���̍ő喇��
	static ComPtr<ID3D12DescriptorHeap> TextureDescHeap;
	static Texture TextureData[2048];
	//�e�N�X�`���̓ǂݍ���
	static int LoadTexture(const char *filename);
	static int LoadTexture2(const std::string& directoryPath, const std::string& filename);
	static int TexManager::LoadTexture3(const std::string &filename);
	//�F�e�N�X�`��
	static int GetColor(XMFLOAT4 color);
	//�����e�N�X�`��
	static int GetStringTexture(TCHAR *moji);
	//�|�X�g�G�t�F�N�g�p�e�N�X�`��
	static int GetPostTexture(float width, float height, XMFLOAT4 color, DXGI_FORMAT format);
	static int GetPostDepthTexture(float width, float height, XMFLOAT4 color);
};

static int LoadTexture(const char *filename) {
	return TexManager::LoadTexture(filename);
}
