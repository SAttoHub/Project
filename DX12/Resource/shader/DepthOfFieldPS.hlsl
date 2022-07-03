#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);
Texture2D<float4> tex3 : register(t2);
Texture2D<float4> DepthTexture : register(t3);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

float4 main(GSOutput input) : SV_TARGET
{
	//仮ヴィネット
	/*float2 uv = input.uv;
	uv = 2.0f * uv - 1.0f;
	c2 *= 1.0 - dot(uv, uv) * 0.2f;*/

	if (Flag == 0.0f) {
		return tex.Sample(smp, input.uv);
	}

	//return c2;
	//float3 hisyakaiArea = float3(100.0f, 150.0f, 100.0f);
	float3 hisyakaiArea = float3(InterpSize, Focus, FocusSize);
	//float3 hisyakaiArea = float3(20.0f, 30.0f, 30.0f);
	float DepthOfField_TotalSize = hisyakaiArea.x * 2.0f + hisyakaiArea.z;
	float2 ParamF = float2(0.0f, 0.0f);
	// 被写界深度のぼかし無しの開始位置を計算
	ParamF.x = hisyakaiArea.x / DepthOfField_TotalSize;
	// 被写界深度のぼかし無しの終了位置を計算
	ParamF.y = (hisyakaiArea.x + hisyakaiArea.z) / DepthOfField_TotalSize;

	float4 Color1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 Color2 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float BlendRate = 0.0f;
	float Fade = 0.0f;
	float4 DepthCol = 0.0f;

	// 深度値を取得する
	DepthCol = DepthTexture.Sample(smp, input.uv);
	float Depth = DepthCol.r;

	// フォーカス情報からぼやけ率を算出
	if (Depth < ParamF.x) {
		Fade = 1.0f - Depth / ParamF.x;
	}
	else {
		if (Depth < ParamF.y) {
			Fade = 0.0f;
		}
		else {
			Fade = (Depth - ParamF.y) / (1.0f - ParamF.y);
		}
	}

	// ぼやけ率から色を算出
	if (Fade < 0.5f) {
		Color1 = tex.Sample(smp, input.uv);
		Color2 = tex2.Sample(smp, input.uv);
		BlendRate = Fade / 0.5f;
	}
	else {
		Color1 = tex2.Sample(smp, input.uv);
		Color2 = tex3.Sample(smp, input.uv);
		BlendRate = (Fade - 0.5f) / 0.5f;
	}

	// 合成した色の値を算出
	float4 Color = lerp(Color1, Color2, BlendRate);

	return Color;

	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	//return tex.Sample(smp, input.uv) * input.color;
}