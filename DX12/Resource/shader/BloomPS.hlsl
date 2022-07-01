#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);	//元テクスチャ
Texture2D<float4> tex2 : register(t1);	//加算テクスチャ
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	float4 Color1 = tex.Sample(smp, input.uv);
	float4 Color2 = tex2.Sample(smp, input.uv);

	return Color1 + Color2 + Color2;

	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	//return tex.Sample(smp, input.uv) * input.color;
}