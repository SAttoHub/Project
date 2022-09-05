cbuffer cbuff0 : register(b0)
{
	matrix mat;
};

struct VSInput
{
	float4 pos1	: POSITION;//位置   
	float4 pos2	: TEXCOORD;//位置   
	float4 color : COLOR;
};

struct VSOutput
{
	float4 pos1	: SV_POSITION;//位置   
	float4 pos2	: TEXCOORD;//位置   
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float4 color : COLOR;
};