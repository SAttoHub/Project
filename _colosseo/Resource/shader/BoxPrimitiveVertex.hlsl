#include "BoxPrimitive.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.pos1 = input.pos1;//ˆÊ’u   
	output.pos2 = input.pos2;//ˆÊ’u   
	/*output.scale = input.scale;
	output.rotate = input.rotate;*/
	output.color = input.color;
	output.Draw3D = input.Draw3D;
	return output;
}