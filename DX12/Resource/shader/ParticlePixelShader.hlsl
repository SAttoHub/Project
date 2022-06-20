#include "ParticleShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	if (input.color.a == 0) discard;

	return tex.Sample(smp, input.uv) * input.color;
}