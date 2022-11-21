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
		// フォント用
		char tag;
		GLYPHMETRICS gm;
		TEXTMETRIC tm;
		UINT Mcode; // 文字コード保存
	};
	TexManager();
	~TexManager();
	void TextureDescHeapCreate();//デスクリプタヒープの生成※必ず3DobjectとspriteBaseの初期化後に呼び出す
	static const int TextureSRVCount; //テクスチャの最大枚数
	static ComPtr<ID3D12DescriptorHeap> TextureDescHeap;
	static Texture TextureData[2048];
	//テクスチャの読み込み
	static int LoadTexture(const char *filename);
	static int LoadTexture2(const std::string& directoryPath, const std::string& filename);
	static int TexManager::LoadTexture3(const std::string &filename);
	//色テクスチャ
	static int GetColor(XMFLOAT4 color);
	//文字テクスチャ
	static int GetStringTexture(TCHAR *moji);
	//ポストエフェクト用テクスチャ
	static int GetPostTexture(float width, float height, XMFLOAT4 color, DXGI_FORMAT format);
	static int GetPostDepthTexture(float width, float height, XMFLOAT4 color);
};

static int LoadTexture(const char *filename) {
	return TexManager::LoadTexture(filename);
}
