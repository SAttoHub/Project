struct VSInput
{
	float4 pos1	: POSITION;//位置   
	float4 pos2	: TEXCOORD;//位置   
	float4 color : COLOR;
	bool Draw3D : FLAG;
};

struct VSOutput
{
	float4 pos1	: SV_POSITION;//位置   
	float4 pos2	: TEXCOORD;//位置   
	float4 color : COLOR;
	bool Draw3D : FLAG;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv  :TEXCOORD;
	float4 color : COLOR;
};

struct GaussianGSOutput {
	float4 pos    : TEXCOORD9;
	float2 texel0 : TEXCOORD0;   // テクセル
	float2 texel1 : TEXCOORD1;   // テクセル
	float2 texel2 : TEXCOORD2;   // テクセル
	float2 texel3 : TEXCOORD3;   // テクセル
	float2 texel4 : TEXCOORD4;   // テクセル
	float2 texel5 : TEXCOORD5;   // テクセル
	float2 texel6 : TEXCOORD6;   // テクセル
	float2 texel7 : TEXCOORD7;   // テクセル

	float2 uv : TEXCOORD8;
	float4 svpos : SV_POSITION;
	float4 color : COLOR;
};