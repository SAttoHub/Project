#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
};

float4 main(GSOutput input) : SV_TARGET
{
	float4 c = tex.Sample(smp, input.uv);
	c.r = c.r * 0.3f;
	c.g = c.g * 0.59f;
	c.b = c.b * 0.11f;

	float gray = c.r + c.g + c.b;

	float4 gray4 = float4(gray, gray, gray, 1);

	return gray4 * input.color;
}