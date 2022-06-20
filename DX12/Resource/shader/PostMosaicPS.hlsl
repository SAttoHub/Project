#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	float2 uv = input.uv * float2(1280, 720);
	uv /= 50.0f;
	uv = floor(uv) * 50.0f;
	uv /= float2(1280, 720);

	return tex.Sample(smp, uv) * input.color;
}