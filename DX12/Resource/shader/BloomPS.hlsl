#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);	//元テクスチャ
Texture2D<float4> tex2 : register(t1);	//加算テクスチャ
Texture2D<float> tex3 : register(t2);	//カメラ深度
Texture2D<float> tex4 : register(t3);	//ブルーム深度
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
};

float4 main(GSOutput input) : SV_TARGET
{
	float4 Color1 = tex.Sample(smp, input.uv);
	float4 Color2 = tex2.Sample(smp, input.uv);

	float depth1 = tex3.Sample(smp, input.uv);
	float depth2 = tex4.Sample(smp, input.uv);

	/*if (depth2 < depth1) {
		return Color1;
	}*/

	return Color1 + Color2 + Color2;

	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	//return tex.Sample(smp, input.uv) * input.color;
}