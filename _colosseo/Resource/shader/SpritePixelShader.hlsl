#include "SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); //0番に設定されたテクスチャ
SamplerState smp : register(s0); //0番に設定されたサンプラー

float4 PSmain(VSOutput input) : SV_TARGET
{
	if (color.a == 0) discard;
	return tex.Sample(smp, input.uv) * color;
}