#include "Tetrahedron.hlsli"

float GetSqrt(float x)
{
	int i = 0;
	float y = 0;
	float z = 0;
	float result = 0;

	if (x == 0) {
		return 0;
	}
	else
	{
		y = 1;
		for (i = 0; i <= 10; i++)
		{
			z = x / y;
			result = (y + z) / 2;
			y = result;
		}
		return result;
	}
}

float3 GetNormal(float3 a, float3 b, float3 c) {
	float3 v0 = a;
	float3 v1 = b;
	float3 v2 = c;

	float3 vv1 = v1 - v0;
	float3 vv2 = v2 - v1;

	//ŠOÏ‚ð‹‚ß‚é
	float3 cross = float3(0, 0, 0);
	cross.x = vv1.y * vv2.z - vv1.z * vv2.y;
	cross.y = vv1.z * vv2.x - vv1.x * vv2.z;
	cross.z = vv1.x * vv2.y - vv1.y * vv2.x;

	float3 Normalize = float3(0, 0, 0);

	float len = GetSqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);

	len = 1.0 / len;
	float x = cross.x * len;
	float y = cross.y * len;
	float z = cross.z * len;

	Normalize.x = x;
	Normalize.y = y;
	Normalize.z = z;

	return Normalize;
}

[maxvertexcount(12)]
void main(
	point VSOutput input[1],
	inout TriangleStream< GSOutput > output
)
{
	const float3 vertexs[12] =
	{
	input[0].pos1.xyz,
	input[0].pos2.xyz,
	input[0].pos3.xyz,
	input[0].pos1.xyz,
	input[0].pos3.xyz,
	input[0].pos4.xyz,
	input[0].pos1.xyz,
	input[0].pos4.xyz,
	input[0].pos2.xyz,
	input[0].pos4.xyz,
	input[0].pos3.xyz,
	input[0].pos2.xyz,
	};
	
	GSOutput element;

	int j = 0;
	int k = 0;
	[unroll]
	for (j = 0; j < 4; j++)
	{
		[unroll]
		for (k = 0; k < 3; k++)
		{
			const int vid = j * 3 + k;
			element.svpos = float4(vertexs[vid], 1.0);

			element.normal = GetNormal(vertexs[j * 3], vertexs[j * 3 + 1], vertexs[j * 3 + 2]);

			element.color = input[0].color;
			element.svpos = mul(mat, element.svpos);
			element.Lighting = input[0].Lighting;

			output.Append(element);
		}
		output.RestartStrip();
	}
}