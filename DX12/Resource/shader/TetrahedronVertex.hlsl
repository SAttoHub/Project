#include "Tetrahedron.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.pos1 = input.pos1;//位置   
	output.pos2 = input.pos2;//位置   
	output.pos3 = input.pos3;//位置   
	output.pos4 = input.pos4;//位置   
	output.color = input.color;
	output.Lighting = input.Lighting;
	return output;
}