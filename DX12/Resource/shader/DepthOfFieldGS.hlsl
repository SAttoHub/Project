#include "PostEffect.hlsli"

cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

[maxvertexcount(4)]
void main(
	point VSOutput input[1],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element[4];

	if (!input[0].Draw3D) {
		for (int i = 0; i < 4; i++) {
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
		element[1].svpos = float4(input[0].pos2.x - 1, input[0].pos2.y + AddY, input[0].pos1.z, 1);
		element[2].svpos = float4(input[0].pos1.x - 1, input[0].pos1.y + AddY, input[0].pos1.z, 1);
		element[3].svpos = float4(input[0].pos2.x - 1, input[0].pos1.y + AddY, input[0].pos1.z, 1);

		element[0].uv = float2(1.0f, 1.0f);
		element[1].uv = float2(0.0f, 1.0f);
		element[2].uv = float2(1.0f, 0.0f);
		element[3].uv = float2(0.0f, 0.0f);

		//element[0].svpos = float4(-0.5, 0.5, 0, 1);
		//element[1].svpos = float4(-0.5, -0.5, 0, 1);
		//element[2].svpos = float4(0.5, 0.5, 0, 1);
		//element[3].svpos = float4(0.5, -0.5, 0, 1);

		[unroll]
		for (int i = 0; i < 4; i++) {
			element[i].color = input[0].color;
			output.Append(element[i]);
		}
	}
	else {
		element[0].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos1.z, 1);
		element[1].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos1.z, 1);
		element[2].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos1.z, 1);
		element[3].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos1.z, 1);
		element[0].uv = float2(1.0f, 1.0f);
		element[1].uv = float2(0.0f, 1.0f);
		element[2].uv = float2(1.0f, 0.0f);
		element[3].uv = float2(0.0f, 0.0f);
		[unroll]
		for (int i = 0; i < 4; i++) {
			element[i].color = input[0].color;
			element[i].svpos = mul(mat, element[i].svpos);
			output.Append(element[i]);
		}
	}
}

