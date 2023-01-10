#pragma once
#include "DirectXBase.h"

// 時間最大数
static const int TIME_MAX_ELEMENT = 2;
// レジスタナンバー
static const int TIME_REGISTER_NUM = 999;

class CommonTime
{
private:
	struct TimeBuffer {
		float Time[TIME_MAX_ELEMENT];
	};
	static float m_Time[TIME_MAX_ELEMENT];
	//定数バッファ
	static ComPtr<ID3D12Resource> m_TimeConstBuffer;
public:
	static void Initialize();
	static void Update();
	static void Draw(int _Index);
	static void CreateRootParameter(CD3DX12_ROOT_PARAMETER* _RootParam);
};

