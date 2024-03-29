#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
};

float4 main(GSOutput input) : SV_TARGET
{
	float4 color = tex.Sample(smp, input.uv);
	return color;

	//if (colors.x == 1.0f && colors.y == 1.0f && colors.z == 1.0f) {
	//	float2 windowSize = float2(1280, 720);

	//	float ripple = 1.158f;
	//	float speed = 1.5f;
	//	float distortion = 0.123f;
	//	//float brightness = 0.555f;
	//	float brightness = 1.0f;


	//	int frameCount = Time;

	//	float2 uv = input.uv;
	//	float time = frameCount * 0.01;
	//	float2 pos = input.svpos.xy / windowSize * 12.0 - 20.0;
	//	float2 tmp = pos;
	//	float speed2 = speed * 2.0;
	//	float inten = 0.015;
	//	float col = 0;

	//	//int Iterations = 8;

	//	for (int i = 0; i < Iterations; ++i)
	//	{
	//		float t = time * (1.0 - (3.2 / (float(i) + speed)));
	//		tmp = pos + float2(
	//			cos(t - tmp.x * ripple) + sin(t + tmp.y * ripple),
	//			sin(t - tmp.y * ripple) + cos(t + tmp.x * ripple)
	//			);
	//		tmp += time;
	//		col += 1.0 / length(float2(pos.x / (sin(tmp.x + t * speed2) / inten), pos.y / (cos(tmp.y + t * speed2) / inten)));
	//	}
	//	col /= float(Iterations);
	//	col = saturate(1.5 - sqrt(col));
	//	uv += col * distortion;
	//	return tex.Sample(smp, uv) + col * brightness;
	//}
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);


	//return float4(1, 0, 0, 1);

	//return tex.Sample(smp, input.uv) * input.color;
}