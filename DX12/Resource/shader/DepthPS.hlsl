#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//---------------��ʊE�[�x�p
	float4 obj = mul(input.worldpos, view); //�r���[�ϊ�
	float4 cp = float4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	float4 cam = mul(cp, view); //�r���[�ϊ�

	float3 hisyakaiArea = float3(100.0f, 150.0f, 100.0f);
	float3 ParamF = float3(0.0f, 0.0f, 0.0f);

	// ��ʊE�[�x�̊J�n�ʒu���v�Z
	ParamF.x = hisyakaiArea.y - hisyakaiArea.z / 2.0f - hisyakaiArea.x;

	// ��ʊE�[�x�̏I���ʒu���v�Z
	ParamF.y = hisyakaiArea.y + hisyakaiArea.z / 2.0f + hisyakaiArea.x;

	// ��ʊE�[�x�͈̔͂̋t�����v�Z
	ParamF.z = 1.0f / (ParamF.y - ParamF.x);

	float4 col = float4(0.0f,0.0f,0.0f,1.0f);

	// ��ʊE�[�x�͈͓̔��� 0.0f �` 1.0f �ɕϊ�
	if ((cam.z - obj.z) < ParamF.x) {
		col.r = 0.0f;
	}
	else if ((cam.z - obj.z) > ParamF.y) {
		col.r = 1.0f;
	}
	else {
		col.r = ((cam.z - obj.z) - ParamF.x) * ParamF.z;
	}

	
	float4 obj_s = mul(input.worldpos, viewproj); //�r���[�ϊ�

	col.r = input.svpos.z / input.svpos.w;

	return col;


	float4 cp_s = float4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	float4 cam_s = mul(cp_s, view); //�r���[�ϊ�
	float MAX_Depth = 300.0f;
	if ((cam_s.z - obj_s.z) < 0.0f) {
		col.b = 0.0f;
	}
	else if ((cam_s.z - obj_s.z) > MAX_Depth) {
		col.b = 1.0f;
	}
	else {
		col.b = (cam_s.z - obj_s.z) / MAX_Depth;
	}

	col.a = 1.0f;

	return float4(input.svpos.z / input.svpos.w, input.svpos.z / input.svpos.w, input.svpos.z / input.svpos.w, 1.0f);
	return input.svpos.z / input.svpos.w;
}