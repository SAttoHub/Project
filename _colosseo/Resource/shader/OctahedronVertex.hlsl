#include "Octahedron.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.pos = input.pos;//ˆÊ’u   
	output.Radius = input.Radius;//ˆÊ’u   
	output.color = input.color;
	output.Lighting = input.Lighting;
	return output;
}