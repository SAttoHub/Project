#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

cbuffer shadow : register(b5)
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


	//float4 cp_shadow = float4(Light_Pos.x, Light_Pos.y, Light_Pos.z, 0.0f);
	//float4 cam_shadow = mul(cp_shadow, Light_view); //�r���[�ϊ�
	//float MAX_Depth = 300.0f; //���C�g����̍ő勗��
	//if ((cam_shadow.z - obj_shadow.z) < 0.0f) {
	//	col.g = 0.0f;
	//}
	//else if ((cam_shadow.z - obj_shadow.z) > MAX_Depth) {
	//	col.g = 1.0f;
	//}
	//else {
	//	col.g = (cam_shadow.z - obj_shadow.z) / MAX_Depth;
	//}

	col.b = 0.0f;
	col.r = 0.0f;
	col.a = 1.0f;

	return float4(input.svpos.z / input.svpos.w, input.svpos.z / input.svpos.w, input.svpos.z / input.svpos.w, 1.0f);
	return input.svpos.z / input.svpos.w;
}