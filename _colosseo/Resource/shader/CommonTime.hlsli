
// ���ԍő吔
static const int TIME_MAX_ELEMENT = 2;
// ���W�X�^�i���o�[
static const int TIME_REGISTER_NUM = 13;

cbuffer TimeBuffer : register(b13)
{
	float Time[TIME_MAX_ELEMENT];
};
