#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);
Texture2D<float4> tex3 : register(t2);
Texture2D<float4> DepthTexture : register(t3);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	//�����B�l�b�g
	/*float2 uv = input.uv;
	uv = 2.0f * uv - 1.0f;
	c2 *= 1.0 - dot(uv, uv) * 0.2f;*/

	//return c2;
	//float3 hisyakaiArea = float3(100.0f, 150.0f, 100.0f);
	float3 hisyakaiArea = float3(1.0f, 1.5f, 1.0f);
	float DepthOfField_TotalSize = hisyakaiArea.x * 2.0f + hisyakaiArea.z;
	float2 ParamF = float2(0.0f, 0.0f);
	// ��ʊE�[�x�̂ڂ��������̊J�n�ʒu���v�Z
	ParamF.x = hisyakaiArea.x / DepthOfField_TotalSize;
	// ��ʊE�[�x�̂ڂ��������̏I���ʒu���v�Z
	ParamF.y = (hisyakaiArea.x + hisyakaiArea.z) / DepthOfField_TotalSize;

	float4 Color1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 Color2 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float BlendRate = 0.0f;
	float Fade = 0.0f;
	float4 DepthCol = 0.0f;

	// �[�x�l���擾����
	DepthCol = DepthTexture.Sample(smp, input.uv);
	float Depth = DepthCol.r;

	// �t�H�[�J�X��񂩂�ڂ₯�����Z�o
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

	// �ڂ₯������F���Z�o
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

	// ���������F�̒l���Z�o
	float4 Color = lerp(Color1, Color2, BlendRate);

	float2 uv = input.uv;
	uv = 2.0f * uv - 1.0f;
	Color *= 1.0 - dot(uv, uv) * 0.2f;

	return Color;

	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	//return tex.Sample(smp, input.uv) * input.color;
}