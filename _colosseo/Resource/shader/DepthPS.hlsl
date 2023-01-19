#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

cbuffer cbuff0 : register(b0)
{
	matrix view; // �r���[�s��
	matrix viewproj; // �r���[�v���W�F�N�V�����s��
	matrix viewproj2; // �r���[�v���W�F�N�V�����s��2
	matrix world; // ���[���h�s��
	float3 cameraPos; // �J�������W�i���[���h���W�j
	float4 InColor;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput
{
	float4 pos	: POSITION;//�ʒu   
	float3 normal : NORMAL;//���_�@��
	float2 uv	: TEXCOORD;//�e�N�X�`���[���W
	min16int4 boneIndices : BONEINDICES; //�{�[���̔ԍ�
	min16int4 boneIndicesB : BONEINDICESB; //�{�[���̔ԍ�
	float4 boneWeights : BONEWEIGHTS; //�{�[���̃X�L���E�F�C�g
	float4 boneWeightsB : BONEWEIGHTSB; //�{�[���̃X�L���E�F�C�g
};

cbuffer time : register(b4)
{
	uint Time;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

float sa(float v1, float v2) {
	float m1 = max(v1, v2);
	float m2 = min(v1, v2);
	return m1 - m2;
}

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcol = tex.Sample(smp, input.uv);
	if (texcol.a < 0.1f) {
		//clip(-1);
		discard;
	}

	if (tex.Sample(smp, input.uv).a == 0.0f) {
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	matrix viewR = view;
	viewR[0][3] = viewR[0][3] - 180.0f;
	viewR[1][3] = viewR[1][3] - 180.0f;
	viewR[2][3] = viewR[2][3] - 180.0f;

	////---------------��ʊE�[�x�p
	float4 obj = mul(input.worldpos, viewR); //�r���[�ϊ�
	float4 cp = float4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	float4 cam = mul(cp, viewR); //�r���[�ϊ�

	float3 DOFArea = float3(InterpSize, Focus, FocusSize);
	float3 ParamF = float3(0.0f, 0.0f, 0.0f);

	// ��ʊE�[�x�̊J�n�ʒu���v�Z
	ParamF.x = DOFArea.y - DOFArea.z / 2.0f - DOFArea.x;

	// ��ʊE�[�x�̏I���ʒu���v�Z
	ParamF.y = DOFArea.y + DOFArea.z / 2.0f + DOFArea.x;

	// ��ʊE�[�x�͈̔͂̋t�����v�Z
	ParamF.z = 1.0f / (ParamF.y - ParamF.x);

	float4 col = float4(0.0f,0.0f,0.0f,1.0f);

	cam.z = cam.z / cam.w;
	obj.z = obj.z / obj.w;

	// ��ʊE�[�x�͈͓̔��� 0.0f �` 1.0f �ɕϊ�
	if (sa(cam.z, obj.z) < ParamF.x) {
		col.r = 0.0f;
	}
	else if (sa(cam.z, obj.z) > ParamF.y) {
		col.r = 1.0f;
	}
	else {
		col.r = (sa(cam.z, obj.z) - ParamF.x) * ParamF.z;
	}

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 5.0f) {
		discard;
	}

	///float sss = sa(cam.z, obj.z);
	//return float4(sss / 500.0f, sss / 500.0f, sss / 500.0f, 1);

	return col;
}