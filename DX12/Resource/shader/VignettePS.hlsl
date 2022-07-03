#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float isUse;
	float Power;
};

float4 main(GSOutput input) : SV_TARGET
{
	float4 Color = tex.Sample(smp, input.uv);
	if (isUse == 0.0f) {
		return Color;
	}
	float2 uv = input.uv;
	uv = 2.0f * uv - 1.0f;
	Color *= 1.0 - dot(uv, uv) * Power;
	return Color;
}