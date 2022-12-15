#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct InstanceData
{
	matrix world; // ���[���h�s��
	float4 InColor;
	float4 uv; // ����E��
	bool DrawFlag;
};

cbuffer cbuff0 : register(b0)
{
	InstanceData data[500];
};


cbuffer matr : register(b6)
{
	matrix view; // �r���[�s��
	matrix viewproj; // �r���[�v���W�F�N�V�����s��
	matrix viewproj2; // �r���[�v���W�F�N�V�����s��2
	float3 cameraPos; // �J�������W�i���[���h���W�j
};

cbuffer shadow : register(b4)
{
	matrix Light_view; // �r���[�s��
	matrix Light_viewproj;
	float3 Light_Pos;
};

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcol = tex.Sample(smp, input.uv);
	if (texcol.a < 0.1f) {
		//clip(-1);
		discard;
	}
	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;

	if (data[index].DrawFlag == false) {
		discard;
	}

	float4 col = float4(0.0f,0.0f,0.0f,1.0f);

	//-----------------�V���h�E�}�b�v�p
	float4 obj_shadow = mul(input.worldpos, Light_viewproj); //�r���[�ϊ�

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 5.0f) {
		discard;
	}

	col.r = input.svpos.z / input.svpos.w;

	return col;
}