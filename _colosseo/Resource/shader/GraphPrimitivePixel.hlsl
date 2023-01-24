#include "GraphPrimitive.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	//float4 aaa = tex.Sample(smp, input.uv);

	/*aaa.x = aaa.x * 1.2f;
	aaa.y = aaa.y * 1.2f;
	aaa.z = aaa.z * 1.2f;
	aaa.w = aaa.w * 1.f;*/

	/*if (aaa.x > 1.0f) aaa.x = 1.0f;
	if (aaa.y > 1.0f) aaa.y = 1.0f;
	if (aaa.z > 1.0f) aaa.z = 1.0f;
	if (aaa.w > 1.0f) aaa.w = 1.0f;*/
	return tex.Sample(smp, input.uv) * input.color;
}
