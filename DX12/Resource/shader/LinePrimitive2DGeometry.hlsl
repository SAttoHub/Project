#include "LinePrimitive2D.hlsli"

[maxvertexcount(2)]
void main(
	point VSOutput input[1],
	inout LineStream< GSOutput > output
)
{
	GSOutput element[2];

	int i = 0;
	for (i = 0; i < 2; i++) {
		element[i].svpos = float4(0, 0, 0, 0);
		element[i].color = float4(1, 1, 1, 1);
	}

	float y1 = input[0].pos1.y;
	float y2 = input[0].pos2.y;
	if (y1 > y2) {
		y1 = input[0].pos2.y;
		y2 = input[0].pos1.y;
	}

	float AddY = 1 - y1 - y2;

	element[0].svpos = float4(input[0].pos1.x - 1, input[0].pos2.y + AddY, input[0].pos1.z, 1);
	element[1].svpos = float4(input[0].pos2.x - 1, input[0].pos1.y + AddY, input[0].pos1.z, 1);
	//element[2].svpos = float4(input[0].pos1.x - 1, input[0].pos1.y + AddY, input[0].pos1.z, 1);
	//element[3].svpos = float4(input[0].pos2.x - 1, input[0].pos1.y + AddY, input[0].pos1.z, 1);

	//element[0].svpos = float4(-0.5, 0.5, 0, 1);
	//element[1].svpos = float4(-0.5, -0.5, 0, 1);
	//element[2].svpos = float4(0.5, 0.5, 0, 1);
	//element[3].svpos = float4(0.5, -0.5, 0, 1);

	[unroll]
	for (i = 0; i < 2; i++) {
		element[i].color = input[0].color;
		output.Append(element[i]);
	}
}



