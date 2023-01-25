#include <Shlwapi.h>
#include <tchar.h>
#include <stdio.h>
#include "TexManager.h"
UINT TexManager::TexNum = 0;
const int TexManager::TextureSRVCount = 2048;
ComPtr<ID3D12DescriptorHeap> TexManager::TextureDescHeap;
TexManager::Texture TexManager::TextureData[2048];

void TexManager::TextureDescHeapCreate() {
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = TextureSRVCount;
	DirectXBase::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&TextureDescHeap));
}

int TexManager::GetColor(XMFLOAT4 color) {
	//colorを0～1に丸める
	color.x < 0 ? color.x = 0 : color.x > 255 ? color.x = 255 : 0;
	color.y < 0 ? color.y = 0 : color.y > 255 ? color.y = 255 : 0;
	color.z < 0 ? color.z = 0 : color.z > 255 ? color.z = 255 : 0;
	color.w < 0 ? color.w = 0 : color.w > 255 ? color.w = 255 : 0;

	color.x = color.x / 255.0f;
	color.y = color.y / 255.0f;
	color.z = color.z / 255.0f;
	color.w = color.w / 255.0f;
	
	//生成済みかどうか
	for (int i = 0; i < 2048; ++i) {
		if (TextureData[i].colorData.x == color.x && TextureData[i].colorData.y == color.y &&
			TextureData[i].colorData.z == color.z && TextureData[i].colorData.w == color.w &&
			TextureData[i].FileName == "ColorTexture") {
			return TextureData[i].TexNum;
		}
	}
	TexNum < TextureSRVCount ? TexNum++ : 0;
	TextureData[TexNum].TexNum = TexNum;
	TextureData[TexNum].colorData.x = color.x;
	TextureData[TexNum].colorData.y = color.y;
	TextureData[TexNum].colorData.z = color.z;
	TextureData[TexNum].colorData.w = color.w;
	TextureData[TexNum].FileName = "ColorTexture";
	//テクスチャデータの生成
	const int TextureWidth = 16;
	const int ImageDataSize = TextureWidth * TextureWidth;
	XMFLOAT4 *imageData = new XMFLOAT4[ImageDataSize];
	//ピクセルの色をセット
	for (int i = 0; i < ImageDataSize; ++i) {
		imageData[i].x = color.x;
		imageData[i].y = color.y;
		imageData[i].z = color.z;
		imageData[i].w = color.w;
	}
	//テクスチャバッファの生成
	HRESULT result;
	CD3DX12_RESOURCE_DESC texresDesc{}; //リソース設定
	texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		TextureWidth,
		(UINT)TextureWidth,
		(UINT16)1,
		(UINT16)1);
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	);
	result = TextureData[TexNum].TextureBuff->WriteToSubresource(
		0,
		nullptr, //全領域へコピー
		imageData, //元データアドレス
		sizeof(XMFLOAT4) * TextureWidth, //1ラインサイズ
		sizeof(XMFLOAT4) * ImageDataSize //全サイズ
	);

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureData[TexNum].TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			TextureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			TexNum,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);


	delete[] imageData;

	return TexNum;
}

int TexManager::GetStringTexture(TCHAR *moji)
{
	// 文字コード取得
	UINT code = 0;
#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
	code = (UINT)*moji;
#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
	if (IsDBCSLeadByte(*moji))
		code = (BYTE)moji[0] << 8 | (BYTE)moji[1];
	else
		code = moji[0];
#endif
	const size_t textSize = 256;
	//char pszText[textSize];
	//WideCharToMultiByte(CP_ACP, 0, moji, -1, pszText, textSize, NULL, NULL);

	char dst[textSize];
	ZeroMemory(&dst[0], textSize);
	WideCharToMultiByte(CP_ACP, 0, &moji[0], lstrlen(moji), &dst[0], textSize, NULL, NULL);
	string* s = new string(dst);

	//生成済みかどうか
	char ss = dst[0];
	for (int i = 0; i < 2048; ++i) {
		if (TextureData[i].Mcode == code) {
			delete s;
			return TextureData[i].TexNum;
		}
	}

	// フォントを読み込む
	// フォントを使えるようにする
	DESIGNVECTOR design = DESIGNVECTOR();
	int count = AddFontResourceEx(
		TEXT("Resource/Corporate-Logo-Bold-ver3.otf"), //ttfファイルへのパス
		FR_PRIVATE,
		&design
	);

	// フォントの生成
	int fontSize = 520; // 260
	int fontWeight = 500;
	LOGFONT lf = {
		fontSize,0,0,0,fontWeight,0,0,0,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		TEXT("コーポレート・ロゴ ver3 Bold")
	};
	HFONT hFont = CreateFontIndirect(&lf);

	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
	//UINT code = (UINT)moji;     // テクスチャに書き込む文字

	// フォントビットマップ取得
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline(
		hdc,
		code,
		GGO_GRAY4_BITMAP,
		&GM,
		0,
		NULL,
		&Mat);
	BYTE *ptr = new BYTE[size];
	GetGlyphOutline(
		hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);


	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	// リソース削除
	//DESIGNVECTOR design;
	//RemoveFontResourceEx(
	//	TEXT("APJapanesefontF.ttf"), //ttfファイルへのパス
	//	FR_PRIVATE,
	//	&design
	//);

	//TexNum < TextureSRVCount ? TexNum++ : 0;
	if (TexNum < TextureSRVCount) {
		TexNum += 1;
		TextureData[size_t(TexNum)].TexNum = TexNum;
		TextureData[size_t(TexNum)].colorData.x = 1.0f;
		TextureData[size_t(TexNum)].colorData.y = 1.0f;
		TextureData[size_t(TexNum)].colorData.z = 1.0f;
		TextureData[size_t(TexNum)].colorData.w = 1.0f;
		TextureData[size_t(TexNum)].FileName = string(&ss).c_str();
		TextureData[size_t(TexNum)].tag = ss;
		TextureData[size_t(TexNum)].Mcode = code;
		TextureData[size_t(TexNum)].gm = GM;
		TextureData[size_t(TexNum)].tm = TM;
	}
	//テクスチャデータの生成
	const int TextureWidth = (GM.gmBlackBoxX + 3) / 4 * 4;
	const int TextureHeight = GM.gmBlackBoxY;
	TextureData[size_t(TexNum)].TextureSize.x = float(TextureWidth);
	TextureData[size_t(TexNum)].TextureSize.y = float(TextureHeight);
	const int ImageDataSize = TextureWidth * TextureHeight;
	XMFLOAT4* imageData = nullptr;
	imageData = new XMFLOAT4[ImageDataSize];
	//ピクセルの色をセット
	int grad = 16;
	/*for (int i = 0; i < ImageDataSize; ++i) {
		imageData[i].x = 1.0f;
		imageData[i].y = 1.0f;
		imageData[i].z = 1.0f;
		imageData[i].w = 1.0f;

		DWORD alpha = ptr[y * fontWidth + x] * 255 / grad;
		pTexBuf[y * fontWidth + x] = (alpha << 24) | 0x00ffffff;
	}*/

	for (int y = 0; y < TextureHeight; y++) {
		for (int x = 0; x < TextureWidth; x++) {
			DWORD alpha = ptr[y * TextureWidth + x] * 255 / grad;
			imageData[y * TextureWidth + x].x = float(0x00ffffff | (alpha << 24));
			imageData[y * TextureWidth + x].y = float(0x00ffffff | (alpha << 24));
			imageData[y * TextureWidth + x].z = float(0x00ffffff | (alpha << 24));
			//imageData[y * TextureWidth + x].w = (alpha << 24) | 0x00ffffff;
			imageData[y * TextureWidth + x].w = float(alpha);

			if (x >= TextureWidth - 2) {
				imageData[y * TextureWidth + x].w = 0.0f;
			}
			if (y >= TextureHeight - 2) {
				imageData[y * TextureWidth + x].w = 0.0f;
			}

			//Alpha = (255 * ptr[x - iOfs_x + iBmp_w * (y - TextureHeight)]) / grad;
			//Color = 0x00ffffff | (Alpha << 24);

			//memcpy((BYTE *)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
		}
	}
	if (s->at(0) == ' ') {
		for (int y = 0; y < TextureHeight; y++) {
			for (int x = 0; x < TextureWidth; x++) {
				imageData[y * TextureWidth + x].w = 0.0f;
			}
		}
	}

	//テクスチャバッファの生成
	HRESULT result;
	CD3DX12_RESOURCE_DESC texresDesc{}; //リソース設定
	texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		TextureWidth,
		(UINT)TextureHeight,
		(UINT16)1,
		(UINT16)1);
	//texresDesc.
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	);
	result = TextureData[TexNum].TextureBuff->WriteToSubresource(
		0,
		nullptr, //全領域へコピー
		imageData, //元データアドレス
		sizeof(XMFLOAT4) * TextureWidth, //1ラインサイズ
		sizeof(XMFLOAT4) * ImageDataSize //全サイズ
	);

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureData[TexNum].TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			TextureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			TexNum,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);

	delete s;
	delete[] imageData;

	return TexNum;
}

int TexManager::GetPostTexture(float width, float height, XMFLOAT4 color, DXGI_FORMAT format)
{
	//colorを0～1に丸める
	color.x < 0 ? color.x = 0 : color.x > 255 ? color.x = 255 : 0;
	color.y < 0 ? color.y = 0 : color.y > 255 ? color.y = 255 : 0;
	color.z < 0 ? color.z = 0 : color.z > 255 ? color.z = 255 : 0;
	color.w < 0 ? color.w = 0 : color.w > 255 ? color.w = 255 : 0;

	color.x = color.x / 255.0f;
	color.y = color.y / 255.0f;
	color.z = color.z / 255.0f;
	color.w = color.w / 255.0f;

	////生成済みかどうか
	//for (int i = 0; i < 2048; ++i) {
	//	if (TextureData[i].colorData.x == color.x && TextureData[i].colorData.y == color.y &&
	//		TextureData[i].colorData.z == color.z && TextureData[i].colorData.w == color.w &&
	//		TextureData[i].FileName == "ColorTexture") {
	//		return TextureData[i].TexNum;
	//	}
	//}
	TexNum < TextureSRVCount ? TexNum++ : 0;
	TextureData[TexNum].TexNum = TexNum;
	TextureData[TexNum].colorData.x = -1.0f;
	TextureData[TexNum].colorData.y = -1.0f;
	TextureData[TexNum].colorData.z = -1.0f;
	TextureData[TexNum].colorData.w = -1.0f;
	TextureData[TexNum].FileName = "PostTexture";
	//テクスチャデータの生成
	const UINT ImageDataSize = UINT(width * height);
	const UINT rowPitch = sizeof(UINT) * UINT(width);
	const UINT depthPitch = rowPitch * UINT(height);
	UINT *imageData = new UINT[ImageDataSize];
	//ピクセルの色をセット
	for (int i = 0; UINT(i) < ImageDataSize; i++) {
		imageData[i] = 0xffffffff;
	}
	//テクスチャバッファの生成
	HRESULT result;
	//CD3DX12_RESOURCE_DESC texresDesc{}; //リソース設定
	//texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	TextureWidth,
	//	(UINT)TextureWidth,
	//	(UINT16)1,
	//	(UINT16)1);
	//result = DirectXBase::dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
	//	nullptr,
	//	IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	//);
	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		format,
		UINT64(width),
		(UINT)height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);
	float col[4] = { color.x, color.y, color.z, color.w };
	//テクスチャバッファの生成
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(format, col),
		IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	);
	assert(SUCCEEDED(result));
	//result = TextureData[TexNum].TextureBuff->WriteToSubresource(
	//	0,
	//	nullptr, //全領域へコピー
	//	imageData, //元データアドレス
	//	sizeof(XMFLOAT4) * TextureWidth, //1ラインサイズ
	//	sizeof(XMFLOAT4) * ImageDataSize //全サイズ
	//);
	result = TextureData[TexNum].TextureBuff->WriteToSubresource(0, nullptr,
		imageData, rowPitch, depthPitch);

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureData[TexNum].TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			TextureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			TexNum,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);


	delete[] imageData;

	return TexNum;
}

int TexManager::GetPostDepthTexture(float width, float height, XMFLOAT4 color)
{
	//colorを0～1に丸める
	color.x < 0 ? color.x = 0 : color.x > 255 ? color.x = 255 : 0;
	color.y < 0 ? color.y = 0 : color.y > 255 ? color.y = 255 : 0;
	color.z < 0 ? color.z = 0 : color.z > 255 ? color.z = 255 : 0;
	color.w < 0 ? color.w = 0 : color.w > 255 ? color.w = 255 : 0;

	color.x = color.x / 255.0f;
	color.y = color.y / 255.0f;
	color.z = color.z / 255.0f;
	color.w = color.w / 255.0f;

	////生成済みかどうか
	//for (int i = 0; i < 2048; ++i) {
	//	if (TextureData[i].colorData.x == color.x && TextureData[i].colorData.y == color.y &&
	//		TextureData[i].colorData.z == color.z && TextureData[i].colorData.w == color.w &&
	//		TextureData[i].FileName == "ColorTexture") {
	//		return TextureData[i].TexNum;
	//	}
	//}
	TexNum < TextureSRVCount ? TexNum++ : 0;
	TextureData[TexNum].TexNum = TexNum;
	TextureData[TexNum].colorData.x = -1.0f;
	TextureData[TexNum].colorData.y = -1.0f;
	TextureData[TexNum].colorData.z = -1.0f;
	TextureData[TexNum].colorData.w = -1.0f;
	TextureData[TexNum].FileName = "PostTexture";
	//テクスチャデータの生成
	const UINT ImageDataSize = UINT(width * height);
	const UINT rowPitch = sizeof(UINT) * UINT(width);
	const UINT depthPitch = rowPitch * UINT(height);
	UINT *imageData = new UINT[ImageDataSize];
	//ピクセルの色をセット
	for (int i = 0; i < int(ImageDataSize); i++) {
		imageData[i] = 0xffffffff;
	}
	//テクスチャバッファの生成
	HRESULT result;
	//CD3DX12_RESOURCE_DESC texresDesc{}; //リソース設定
	//texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	TextureWidth,
	//	(UINT)TextureWidth,
	//	(UINT16)1,
	//	(UINT16)1);
	//result = DirectXBase::dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
	//	nullptr,
	//	IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	//);
	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		UINT(width),
		UINT(height),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	float col[4] = { color.x, color.y, color.z, color.w };
	//テクスチャバッファの生成
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	);
	assert(SUCCEEDED(result));
	//result = TextureData[TexNum].TextureBuff->WriteToSubresource(
	//	0,
	//	nullptr, //全領域へコピー
	//	imageData, //元データアドレス
	//	sizeof(XMFLOAT4) * TextureWidth, //1ラインサイズ
	//	sizeof(XMFLOAT4) * ImageDataSize //全サイズ
	//);
	result = TextureData[TexNum].TextureBuff->WriteToSubresource(0, nullptr,
		imageData, rowPitch, depthPitch);

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureData[TexNum].TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			TextureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			TexNum,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);


	delete[] imageData;

	return TexNum;
}

TexManager::TexManager() {
	TexNum = 0;
}

TexManager::~TexManager()
{
	/*for (int i = 0; i < TexNum; ++i) {
		TextureData[i].TextureBuff.Reset();
		delete TextureData[i].img;
	}*/
}

int TexManager::LoadTexture(const char *filename) {
	//読み込み済みなら既にあるものを返す
	for (int i = 0; i < 2048; ++i) {
		if (TextureData[i].FileName == filename) {
			return TextureData[i].TexNum;
		}
	}
	HRESULT result;
	TexNum < TextureSRVCount ? TexNum++ : 0;
	TextureData[TexNum].TexNum = TexNum;
	TextureData[TexNum].FileName = filename;
	//TexMetadata metadata{};
	//ScratchImage scratchImg{};
	//  char -> wchar_t
	size_t newsize = strlen(filename) + 1;
	wchar_t *FileName = new wchar_t[newsize];
	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, FileName, newsize, filename, _TRUNCATE);

	//WICテクスチャのロード
	result = LoadFromWICFile(
		FileName,
		WIC_FLAGS_NONE,
		&TextureData[TexNum].metadata, TextureData[TexNum].scratchImg
	);
	TextureData[TexNum].img = TextureData[TexNum].scratchImg.GetImage(0, 0, 0);
	//テクスチャバッファの作成
	CD3DX12_RESOURCE_DESC texresDesc{}; //リソース設定
	texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		TextureData[TexNum].metadata.format,
		TextureData[TexNum].metadata.width,
		(UINT)TextureData[TexNum].metadata.height,
		(UINT16)TextureData[TexNum].metadata.arraySize,
		(UINT16)TextureData[TexNum].metadata.mipLevels
	);
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	);

	result = TextureData[TexNum].TextureBuff->WriteToSubresource(
		0,
		nullptr, //全領域へコピー
		TextureData[TexNum].img->pixels, //元データアドレス
		(UINT)TextureData[TexNum].img->rowPitch, //1ラインサイズ
		(UINT)TextureData[TexNum].img->slicePitch //全サイズ
	);
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = TextureData[TexNum].metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureData[TexNum].TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			TextureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			TexNum,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
	return TexNum;
}

int TexManager::LoadTexture2(const std::string &directoryPath, const std::string &filename)
{
	HRESULT result;
	TexNum < TextureSRVCount ? TexNum++ : 0;
	TextureData[TexNum].TexNum = TexNum;
	string filePath = directoryPath + filename;
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wfilepath, _countof(wfilepath));

	//WICテクスチャのロード
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&TextureData[TexNum].metadata, TextureData[TexNum].scratchImg
	);
	TextureData[TexNum].img = TextureData[TexNum].scratchImg.GetImage(0, 0, 0);
	//テクスチャバッファの作成
	CD3DX12_RESOURCE_DESC texresDesc{}; //リソース設定
	texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		TextureData[TexNum].metadata.format,
		TextureData[TexNum].metadata.width,
		(UINT)TextureData[TexNum].metadata.height,
		(UINT16)TextureData[TexNum].metadata.arraySize,
		(UINT16)TextureData[TexNum].metadata.mipLevels
	);
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	);

	result = TextureData[TexNum].TextureBuff->WriteToSubresource(
		0,
		nullptr, //全領域へコピー
		TextureData[TexNum].img->pixels, //元データアドレス
		(UINT)TextureData[TexNum].img->rowPitch, //1ラインサイズ
		(UINT)TextureData[TexNum].img->slicePitch //全サイズ
	);
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = TextureData[TexNum].metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureData[TexNum].TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			TextureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			TexNum,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
	return TexNum;
}

int TexManager::LoadTexture3(const std::string &filename)
{
	HRESULT result;
	TexNum < TextureSRVCount ? TexNum++ : 0;
	TextureData[TexNum].TexNum = TexNum;

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, wfilepath, _countof(wfilepath));

	//WICテクスチャのロード
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&TextureData[TexNum].metadata, TextureData[TexNum].scratchImg
	);
	TextureData[TexNum].img = TextureData[TexNum].scratchImg.GetImage(0, 0, 0);
	//テクスチャバッファの作成
	CD3DX12_RESOURCE_DESC texresDesc{}; //リソース設定
	texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		TextureData[TexNum].metadata.format,
		TextureData[TexNum].metadata.width,
		(UINT)TextureData[TexNum].metadata.height,
		(UINT16)TextureData[TexNum].metadata.arraySize,
		(UINT16)TextureData[TexNum].metadata.mipLevels
	);
	result = DirectXBase::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&TextureData[TexNum].TextureBuff)
	);

	result = TextureData[TexNum].TextureBuff->WriteToSubresource(
		0,
		nullptr, //全領域へコピー
		TextureData[TexNum].img->pixels, //元データアドレス
		(UINT)TextureData[TexNum].img->rowPitch, //1ラインサイズ
		(UINT)TextureData[TexNum].img->slicePitch //全サイズ
	);
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = TextureData[TexNum].metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//シェーダーリソースビュー作成
	DirectXBase::dev->CreateShaderResourceView(TextureData[TexNum].TextureBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			TextureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			TexNum,
			DirectXBase::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
	return TexNum;
}
