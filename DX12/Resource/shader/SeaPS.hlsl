#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

cbuffer time : register(b4)
{
	uint Time;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

#define  Iterations  24

float4 main(VSOutput input) : SV_TARGET
{
	float2 windowSize = float2(20, 20) * 50;

	float ripple = 1.158f;
	float speed = 1.5f;
	float distortion = 0.123f;
	//float brightness = 0.555f;
	float brightness = 1.0f;

	int frameCount = Time;

	float2 uv = input.uv;
	float time = float(frameCount) * 0.01f;
	float2 pos = (((input.worldpos.xz / 10.0f) + float2(10,10)) * 50) / windowSize * 12.0 - 20.0;
	float2 tmp = pos;
	float speed2 = speed * 2.0;
	float inten = 0.015;
	float col = 0;

	//int Iterations = 8;

	for (int i = 0; i < Iterations; ++i)
	{
		float t = time * (1.0 - (3.2 / (float(i) + speed)));
		tmp = pos + float2(
		cos(t - tmp.x * ripple) + sin(t + tmp.y * ripple),
			sin(t - tmp.y * ripple) + cos(t + tmp.x * ripple)
			);
		tmp += time;
		col += 1.0 / length(float2(pos.x / (sin(tmp.x + t * speed2) / inten), pos.y / (cos(tmp.y + t * speed2) / inten)));
	}
	col /= float(Iterations);
	col = saturate(1.5 - sqrt(col));
	uv += col * distortion;


	// ���_���王�_�ւ̕����x�N�g��(�����x�N�g��)
	float3 eyedir = normalize(input.worldpos.xyz - cameraPos);
	//�����}�b�v���Q�Ƃ��A�������擾����
	float4 heightcolor = tex.Sample(smp, uv);
	float h = heightcolor.r;
	//�����̃p�����[�^�@�傫������ƍ������傫���Ȃ�
   float  m_Param = 0.016f * 2.0f;
   //�e�N�Z���𒸓_���W�n�ł̎����x�N�g�������ɏd�݂����Ă��炷�B
   float2 Tex = uv + m_Param * h * eyedir.xy;


   return (tex.Sample(smp, Tex) + col * brightness) * InColor;

   //return float4(149.0f / 255.0f, 170.0f / 255.0f, 169.0f / 255.0f, 1.0f);

   //return float4(1.0f, 1.0f, 1.0f, 1.0f);
}