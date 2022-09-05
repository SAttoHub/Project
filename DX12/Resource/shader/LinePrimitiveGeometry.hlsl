#include "LinePrimitive.hlsli"
//
//[maxvertexcount(2)]
//void main(
//	point VSOutput input[1],
//	inout LineStream< GSOutput > output
//)
//{
//	GSOutput element[2];
//
//	element[0].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos1.z, 1);
//	element[1].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos2.z, 1);
//
//	for (int i = 0; i < 2; i++) {
//		element[i].color = input[0].color;
//		element[i].svpos = mul(mat, element[i].svpos);
//		output.Append(element[i]);
//	}
//}
//

[maxvertexcount(24)]
void main(
	point VSOutput input[1],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element[24];

	for (int i = 0; i < 24; i++) {
		element[i].svpos = float4(0, 0, 0, 0);
		element[i].uv = float2(0, 0);
		element[i].color = float4(0, 0, 0, 0);
	}

	// pos‚ðŒ´“_’†S‚É‚È‚é‚æ‚¤‚É•ÏŠ·
	//float genX = (input[0].pos1.x + input[0].pos2.x) / 2;
	//float genY = (input[0].pos1.y + input[0].pos2.y) / 2;
	//float genZ = (input[0].pos1.z + input[0].pos2.z) / 2;

	input[0].pos1.x = input[0].pos1.x + 0.15;
	input[0].pos1.y = input[0].pos1.y + 0.01;
	input[0].pos1.z = input[0].pos1.z + 0.15;
	input[0].pos2.x = input[0].pos2.x - 0.15;
	input[0].pos2.y = input[0].pos2.y - 0.01;
	input[0].pos2.z = input[0].pos2.z - 0.15;

	float hoge = 5.25f;

	element[0].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos1.z, 1);
	element[1].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos1.z, 1);
	element[2].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos1.z, 1);
	element[3].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos1.z, 1);
	element[0].uv = float2(0.0f, 1.0f);
	element[1].uv = float2(0.0f, 0.0f);
	element[2].uv = float2(1.0f, 1.0f);
	element[3].uv = float2(1.0f, 0.0f);

	element[5].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos2.z, 1);
	element[6].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos2.z, 1);
	element[7].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos2.z, 1);
	element[4].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos2.z, 1);
	element[4].uv = float2(0.0f, 1.0f);
	element[5].uv = float2(0.0f, 0.0f);
	element[6].uv = float2(1.0f, 1.0f);
	element[7].uv = float2(1.0f, 0.0f);

	element[8].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos2.z, 1);
	element[9].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos1.z, 1);
	element[10].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos2.z, 1);
	element[11].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos1.z, 1);
	element[8].uv = float2(0.0f, 1.0f);
	element[9].uv = float2(0.0f, 0.0f);
	element[10].uv = float2(1.0f, 1.0f);
	element[11].uv = float2(1.0f, 0.0f);

	element[12].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos1.z, 1);
	element[13].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos2.z, 1);
	element[14].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos1.z, 1);
	element[15].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos2.z, 1);
	element[12].uv = float2(0.0f, 1.0f);
	element[13].uv = float2(0.0f, 0.0f);
	element[14].uv = float2(1.0f, 1.0f);
	element[15].uv = float2(1.0f, 0.0f);

	element[16].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos2.z, 1);
	element[17].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos2.z, 1);
	element[18].svpos = float4(input[0].pos1.x, input[0].pos2.y, input[0].pos1.z, 1);
	element[19].svpos = float4(input[0].pos2.x, input[0].pos2.y, input[0].pos1.z, 1);
	element[16].uv = float2(0.0f, 1.0f);
	element[17].uv = float2(0.0f, 0.0f);
	element[18].uv = float2(1.0f, 1.0f);
	element[19].uv = float2(1.0f, 0.0f);

	element[20].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos1.z, 1);
	element[21].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos1.z, 1);
	element[22].svpos = float4(input[0].pos1.x, input[0].pos1.y, input[0].pos2.z, 1);
	element[23].svpos = float4(input[0].pos2.x, input[0].pos1.y, input[0].pos2.z, 1);
	element[20].uv = float2(0.0f, 1.0f);
	element[21].uv = float2(0.0f, 0.0f);
	element[22].uv = float2(1.0f, 1.0f);
	element[23].uv = float2(1.0f, 0.0f);

	[unroll]
	for (int i = 0; i < 6; i++) {
		[unroll]
		for (int j = 0; j < 4; j++) {
			const int a = i * 4 + j;
			element[a].color = input[0].color;

			element[a].svpos = mul(mat, element[a].svpos);

			output.Append(element[a]);
		}
		output.RestartStrip();
	}

}

