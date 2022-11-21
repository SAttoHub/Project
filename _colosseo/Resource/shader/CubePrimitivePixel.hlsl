#include "CubePrimitive.hlsli"
//Texture2D<float4> tex : register(t0);
//SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	if (input.Lighting == false) {
		return input.color;
	}
	else {
	//return tex.Sample(smp, input.uv) * input.color;

	float3 light = normalize(float3(0, -1, 1));
	float diffuse = dot(-light, input.normal);

	if (diffuse < 0)diffuse = 0;

	float brightness = diffuse + 0.3f;

	//float4 texcolor = float4(tex.Sample(smp, input.uv));

	return float4(input.color.rgb * brightness, input.color.a);
	}
}