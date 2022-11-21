#include "LinePrimitive2D.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.pos1 = input.pos1;//ˆÊ’u   
	output.pos2 = input.pos2;//ˆÊ’u   
	output.color = input.color;
	return output;
}