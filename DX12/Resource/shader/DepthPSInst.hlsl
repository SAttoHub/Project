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

cbuffer time : register(b4)
{
	uint Time;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};


float4 main(VSOutput input) : SV_TARGET
{
	float4 texcol = tex.Sample(smp, input.uv);
	if (texcol.a < 0.1f) {
		clip(-1);
		//discard;
	}

	if (tex.Sample(smp, input.uv).a == 0.0f) {
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;

	if (data[index].DrawFlag == false) {
		discard;
	}

	////---------------��ʊE�[�x�p
	float4 obj = mul(input.worldpos, view); //�r���[�ϊ�
	float4 cp = float4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	float4 cam = mul(cp, view); //�r���[�ϊ�

	float3 DOFArea = float3(InterpSize, Focus, FocusSize);
	float3 ParamF = float3(0.0f, 0.0f, 0.0f);

	// ��ʊE�[�x�̊J�n�ʒu���v�Z
	ParamF.x = DOFArea.y - DOFArea.z / 2.0f - DOFArea.x;

	// ��ʊE�[�x�̏I���ʒu���v�Z
	ParamF.y = DOFArea.y + DOFArea.z / 2.0f + DOFArea.x;

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

	return col;
}