
// 時間最大数
static const int TIME_MAX_ELEMENT = 2;
// レジスタナンバー
static const int TIME_REGISTER_NUM = 13;

cbuffer TimeBuffer : register(b13)
{
	float Time[TIME_MAX_ELEMENT];
};
