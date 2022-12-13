#include "LinePrimitive.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	/*float4 aaa = tex.Sample(smp, input.uv);
	if (aaa.a < 0.5f) {
		discard;
	}
	if (aaa.r < 0.3f && aaa.g < 0.3f && aaa.b < 0.3f) {
		discard;
	}
	return aaa;*/
	//return input.color;
	return float4(0.231f, 0.176f, 0.11f, 0.7f);
}