#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

cbuffer shadow : register(b4)
{
	matrix Light_view; // �r���[�s��
	matrix Light_viewproj;
	float3 Light_Pos;
};

float4 main(VSOutput input) : SV_TARGET
{
	float4 col = float4(0.0f,0.0f,0.0f,1.0f);

	//-----------------�V���h�E�}�b�v�p
	float4 obj_shadow = mul(input.worldpos, Light_viewproj); //�r���[�ϊ�

	col.r = input.svpos.z / input.svpos.w;

	return col;
}