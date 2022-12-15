#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);
Texture2D<float4> tex3 : register(t2);
Texture2D<float4> DepthTexture : register(t3);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

float sa(float v1, float v2) {
	if (v1 > v2) {
		return v1 - v2;
	}
	else {
		return v2 - v1;
	}
	return 0.0f;
}

float4 main(GSOutput input) : SV_TARGET
{
	if (Flag == 0.0f) {
		return tex.Sample(smp, input.uv);
	}

	float3 DOFArea = float3(InterpSize, Focus, FocusSize);
	float DepthOfField_TotalSize = DOFArea.x * 2.0f + DOFArea.z;
	float2 ParamF = float2(0.0f, 0.0f);
	// ��ʊE�[�x�̂ڂ��������̊J�n�ʒu���v�Z
	ParamF.x = DOFArea.x / DepthOfField_TotalSize;
	// ��ʊE�[�x�̂ڂ��������̏I���ʒu���v�Z
	ParamF.y = (DOFArea.x + DOFArea.z) / DepthOfField_TotalSize;

	float4 Color1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 Color2 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float BlendRate = 0.0f;
	float Fade = 0.0f;
	float4 DepthCol = 0.0f;

	// �[�x�l���擾����
	DepthCol = DepthTexture.Sample(smp, input.uv);
	float Depth = DepthCol.r;
	//return float4(Depth, Depth, Depth, 1);

	// �t�H�[�J�X��񂩂�ڂ₯�����Z�o
	if (Depth < ParamF.x) {
		Fade = 1.0f - Depth / ParamF.x;
	}
	else {
		if (Depth < ParamF.y) {
			Fade = 0.0f;
		}
		else {
			Fade = sa(Depth, ParamF.y) / sa(1.0f, ParamF.y);
		}
	}

	// �ڂ₯������F���Z�o
	if (Fade < 0.5f) {
		Color1 = tex.Sample(smp, input.uv);
		Color2 = tex2.Sample(smp, input.uv);
		BlendRate = Fade / 0.5f;
	}
	else {
		Color1 = tex2.Sample(smp, input.uv);
		Color2 = tex3.Sample(smp, input.uv);
		BlendRate = (Fade - 0.5f) / 0.5f;
	}

	//return float4(1.0f * BlendRate, 0.0f, 1.0f * (1.0f - BlendRate), 1.0f);

	// ���������F�̒l���Z�o
	float4 Color = lerp(Color1, Color2, BlendRate);

	return Color;
}