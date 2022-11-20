#include "BoxPrimitive.hlsli"

[maxvertexcount(4)]
void main(
	point VSOutput input[1],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element[4];

	int i = 0;
	if (!input[0].Draw3D) {
		for (i = 0; i < 4; i++) {
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

		// âÒì]óp
		// posÇå¥ì_íÜêSÇ…Ç»ÇÈÇÊÇ§Ç…ïœä∑
		/*float X1 = input[0].pos1.x - 1;
		float X2 = input[0].pos2.x - 1;
		float Y1 = input[0].pos1.y + AddY;
		float Y2 = input[0].pos2.y + AddY;

		float genX = (X1 + X2) / 2.0f;
		float genY = (Y1 + Y2) / 2.0f;

		float Xprop1 = X1 - genX;
		float Xprop2 = X2 - genX;
		float Yprop1 = Y1 - genY;
		float Yprop2 = Y2 - genY;*/





		element[0].svpos = float4(input[0].pos1.x - 1, input[0].pos2.y + AddY, input[0].pos1.z, 1);
		element[1].svpos = float4(input[0].pos2.x - 1, input[0].pos2.y + AddY, input[0].pos1.z, 1);
		element[2].svpos = float4(input[0].pos1.x - 1, input[0].pos1.y + AddY, input[0].pos1.z, 1);
		element[3].svpos = float4(input[0].pos2.x - 1, input[0].pos1.y + AddY, input[0].pos1.z, 1);

		//element[0].svpos = float4(-0.5, 0.5, 0, 1);
		//element[1].svpos = float4(-0.5, -0.5, 0, 1);
		//element[2].svpos = float4(0.5, 0.5, 0, 1);
		//element[3].svpos = float4(0.5, -0.5, 0, 1);

		[unroll]
		for (i = 0; i < 4; i++) {
			element[i].color = input[0].color;
			output.Append(element[i]);
		}
	}
	else {
		element[0].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos1.z, 1);
		element[1].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos1.z, 1);
		element[2].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos1.z, 1);
		element[3].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos1.z, 1);
		[unroll]
		for (i = 0; i < 4; i++) {
			element[i].color = input[0].color;
			element[i].svpos = mul(mat, element[i].svpos);
			output.Append(element[i]);
		}
	}
}

