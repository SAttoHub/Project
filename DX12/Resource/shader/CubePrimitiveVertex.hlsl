#include "CubePrimitive.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.pos1	= input.pos1;//�ʒu   
	output.pos2	= input.pos2;//�ʒu 
	output.scale = input.scale;
	output.rotate = input.rotate;
	output.color = input.color;
	output.Lighting = input.Lighting;
	return output;
}