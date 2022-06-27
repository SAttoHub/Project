#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

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

float4 main(GaussianGSOutput input) : SV_TARGET
{
	//return tex.Sample(smp, input.uv);

	float4 Out = float4(0.0f, 0.0f, 0.0f, 0.0f);

	Out += (tex.Sample(smp, input.texel0) + tex.Sample(smp, float2(input.texel7.x + g_OffsetX, input.texel7.y))) * g_Weight0;
	Out += (tex.Sample(smp, input.texel1) + tex.Sample(smp, float2(input.texel6.x + g_OffsetX, input.texel6.y))) * g_Weight1;
	Out += (tex.Sample(smp, input.texel2) + tex.Sample(smp, float2(input.texel5.x + g_OffsetX, input.texel5.y))) * g_Weight2;
	Out += (tex.Sample(smp, input.texel3) + tex.Sample(smp, float2(input.texel4.x + g_OffsetX, input.texel4.y))) * g_Weight3;
	Out += (tex.Sample(smp, input.texel4) + tex.Sample(smp, float2(input.texel3.x + g_OffsetX, input.texel3.y))) * g_Weight4;
	Out += (tex.Sample(smp, input.texel5) + tex.Sample(smp, float2(input.texel2.x + g_OffsetX, input.texel2.y))) * g_Weight5;
	Out += (tex.Sample(smp, input.texel6) + tex.Sample(smp, float2(input.texel1.x + g_OffsetX, input.texel1.y))) * g_Weight6;
	Out += (tex.Sample(smp, input.texel7) + tex.Sample(smp, float2(input.texel0.x + g_OffsetX, input.texel0.y))) * g_Weight7;


	/*float4 output = float4(0.0f, 0.0f, 0.0f, 0.0f);

	output += g_Weight0 * (tex.Sample(smp, input.texel0) + tex.Sample(smp, input.texel7 + float2(g_OffsetX, 0.0f)));
	output += g_Weight1 * (tex.Sample(smp, input.texel1) + tex.Sample(smp, input.texel6 + float2(g_OffsetX, 0.0f)));
	output += g_Weight2 * (tex.Sample(smp, input.texel2) + tex.Sample(smp, input.texel5 + float2(g_OffsetX, 0.0f)));
	output += g_Weight3 * (tex.Sample(smp, input.texel3) + tex.Sample(smp, input.texel4 + float2(g_OffsetX, 0.0f)));
																									 
	output += g_Weight4 * (tex.Sample(smp, input.texel4) + tex.Sample(smp, input.texel3 + float2(g_OffsetX, 0.0f)));
	output += g_Weight5 * (tex.Sample(smp, input.texel5) + tex.Sample(smp, input.texel2 + float2(g_OffsetX, 0.0f)));
	output += g_Weight6 * (tex.Sample(smp, input.texel6) + tex.Sample(smp, input.texel1 + float2(g_OffsetX, 0.0f)));
	output += g_Weight7 * (tex.Sample(smp, input.texel7) + tex.Sample(smp, input.texel0 + float2(g_OffsetX, 0.0f)));

	return output;*/

	//Out = float4(1.0f, 0.0f, 0.0f, 1.0f);
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return Out;
}