#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float isUse;
	float Power;
};

float3 PositivePow(float3 base, float3 power)
{
	return pow(max(abs(base), float3(1.192092896e-07, 1.192092896e-07, 1.192092896e-07)), power);
}

float4 main(GSOutput input) : SV_TARGET
{
	float4 Color = tex.Sample(smp, input.uv);

	// ƒŠƒjƒA‚Å‚àƒKƒ“ƒ}‚Å‚à“¯‚¶‚É‚·‚é
	float3 col = float3(Color.r, Color.g, Color.b);

	float3 sRGBLo = col * 12.92;
	float3 sRGBHi = (PositivePow(col, half3(1.0 / 2.4, 1.0 / 2.4, 1.0 / 2.4)) * 1.055) - 0.055;
	float3 sRGB = (col <= 0.0031308) ? sRGBLo : sRGBHi;

	col = sRGB;

	float3 linearRGBLo = col / 12.92;
	float3 linearRGBHi = PositivePow((col + 0.055) / 1.055, half3(2.4, 2.4, 2.4));
	float3 linearRGB = (col <= 0.04045) ? linearRGBLo : linearRGBHi;
	Color = float4(linearRGB.r, linearRGB.g, linearRGB.b, 1.0f);

	//

	if (isUse == 0.0f) {
		return Color;
	}
	float2 uv = input.uv;
	// UV‚ð(0,0)(1,1)‚©‚ç(-1,-1),(1,1)‚É
	uv = 2.0f * uv - 1.0f;
	Color *= 1.0 - dot(uv, uv) * Power;
	return Color;
}