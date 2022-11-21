
cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix matBillboard;
};

struct VSInput
{
	float4 pos1	: POSITION;//位置   
	float4 pos2	: POSITIONA;//位置   
	float4 pos3	: POSITIONB;//位置   
	float4 pos4	: POSITIONC;//位置   
	float4 color : COLOR;
	bool Lighting : LIGHTING;
};

struct VSOutput
{
	float4 pos1	: SV_POSITION;//位置   
	float4 pos2	: TEXCOORDA;//位置   
	float4 pos3	: TEXCOORDB;//位置   
	float4 pos4	: TEXCOORDC;//位置   
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