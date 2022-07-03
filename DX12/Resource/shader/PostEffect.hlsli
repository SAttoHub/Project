struct VSInput
{
	float4 pos1	: POSITION;//�ʒu   
	float4 pos2	: TEXCOORD;//�ʒu   
	float4 color : COLOR;
	bool Draw3D : FLAG;
};

struct VSOutput
{
	float4 pos1	: SV_POSITION;//�ʒu   
	float4 pos2	: TEXCOORD;//�ʒu   
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
	float2 texel0 : TEXCOORD0;   // �e�N�Z��
	float2 texel1 : TEXCOORD1;   // �e�N�Z��
	float2 texel2 : TEXCOORD2;   // �e�N�Z��
	float2 texel3 : TEXCOORD3;   // �e�N�Z��
	float2 texel4 : TEXCOORD4;   // �e�N�Z��
	float2 texel5 : TEXCOORD5;   // �e�N�Z��
	float2 texel6 : TEXCOORD6;   // �e�N�Z��
	float2 texel7 : TEXCOORD7;   // �e�N�Z��

	float2 uv : TEXCOORD8;
	float4 svpos : SV_POSITION;
	float4 color : COLOR;
};