#include "ParticleShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float scale : SCALE, float4 color : COLOR)
{
	VSOutput output;
	output.pos = pos;
	output.color = color;
	output.scale = scale;
	return output;
}