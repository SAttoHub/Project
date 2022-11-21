#include "Octahedron.hlsli"

float4 main(GSOutput input) : SV_TARGET
{
	if (input.Lighting == false) {
		return input.color;
	}
	else 
	{
		float3 light = normalize(float3(0, -1, 1));
		float diffuse = dot(-light, input.normal);

		if (diffuse < 0)diffuse = 0;

		float brightness = diffuse + 0.3f;

		return float4(input.color.rgb * brightness, input.color.a);
	}
}