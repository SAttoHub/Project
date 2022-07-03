#include "PostEffect.hlsli"

cbuffer cbuff0 : register(b0)
{
	matrix mat;
};

cbuffer cbBuffer2 : register(b2)
{
	float  g_Weight0;    // 重み
	float  g_Weight1;    // 重み
	float  g_Weight2;    // 重み
	float  g_Weight3;    // 重み
	float  g_Weight4;    // 重み
	float  g_Weight5;    // 重み
	float  g_Weight6;    // 重み
	float  g_Weight7;    // 重み
	float  g_Width;    // サーフェスの横サイズ
	float  g_Height;    // サーフェスの縦サイズ
	float  g_OffsetX;
	float  g_OffsetY;
};

[maxvertexcount(4)]
void main(
	point VSOutput input[1],
	inout TriangleStream< GaussianGSOutput > output
)
{
	GaussianGSOutput element[4];

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

		[unroll]
		for (int i = 0; i < 4; i++) {
			element[i].color = input[0].color;
			float2 texel = element[i].uv;

			element[i].pos = float4(element[i].svpos.x, element[i].svpos.y, 0.0f, 0.0f);
			element[i].texel0 = texel + float2(-1.0f / g_Width, 0.0f);
			element[i].texel1 = texel + float2(-3.0f / g_Width, 0.0f);
			element[i].texel2 = texel + float2(-5.0f / g_Width, 0.0f);
			element[i].texel3 = texel + float2(-7.0f / g_Width, 0.0f);
			element[i].texel4 = texel + float2(-9.0f / g_Width, 0.0f);
			element[i].texel5 = texel + float2(-11.0f / g_Width, 0.0f);
			element[i].texel6 = texel + float2(-13.0f / g_Width, 0.0f);
			element[i].texel7 = texel + float2(-15.0f / g_Width, 0.0f);

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
			float2 texel = element[i].uv;

			element[i].pos = float4(0.0f, 0.0f,0.0f, 1.0f);
			element[i].texel0 = texel + float2(-1.0f / g_Width, 0.0f);
			element[i].texel1 = texel + float2(-3.0f / g_Width, 0.0f);
			element[i].texel2 = texel + float2(-5.0f / g_Width, 0.0f);
			element[i].texel3 = texel + float2(-7.0f / g_Width, 0.0f);
			element[i].texel4 = texel + float2(-9.0f / g_Width, 0.0f);
			element[i].texel5 = texel + float2(-11.0f / g_Width, 0.0f);
			element[i].texel6 = texel + float2(-13.0f / g_Width, 0.0f);
			element[i].texel7 = texel + float2(-15.0f / g_Width, 0.0f);

			output.Append(element[i]);
		}
	}
}

