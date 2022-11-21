
cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix matBillboard;
};

struct VSInput
{
	float4 pos	: POSITION;//位置   
	float Radius : TEXCOORD;//位置   
	float4 color : COLOR;
	bool Lighting : LIGHTING;
};

struct VSOutput
{
	float4 pos	: SV_POSITION;//位置   
	float Radius : TEXCOORD;//位置   
	float4 color : COLOR;
	bool Lighting : LIGHTING;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	bool Lighting : LIGHTING;
};