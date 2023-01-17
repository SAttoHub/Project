#include "FBX.hlsli"
#include "CommonTime.hlsli"
//�X�L�j���O��̒��_�E�@��������
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

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

//�{�[���̍ő吔
static const int MAX_BONES = 61;

cbuffer skinning:register(b3)//�{�[���̃X�L�j���O�s�񂪓���
{
	matrix matSkinning[MAX_BONES];
};

cbuffer shadow : register(b5)
{
	matrix Light_view; // �r���[�s��
	matrix Light_viewproj;
	float3 Light_Pos;
};

// ��Ԋ֐�
float interpolate(float a, float b, float x) {
	float PI = 3.1415926;

	float f = (1.0 - cos(x * PI)) * 0.5;
	return a * (1.0 - f) + b * f;
}

// ��������
float rnd(float2 p) {
	return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

// ��ԗ���
float irnd(float2 p) {
	float2 i = floor(p);
	float2 f = frac(p);
	float4 v = float4(rnd(float2(i.x, i.y)),
		rnd(float2(i.x + 1.0, i.y)),
		rnd(float2(i.x, i.y + 1.0)),
		rnd(float2(i.x + 1.0, i.y + 1.0)));
	return interpolate(interpolate(v.x, v.y, f.x), interpolate(v.z, v.w, f.x), f.y);
}

// �m�C�Y����
float noise(float2 p) {
	int   oct = 8;
	float per = 0.5;

	float t = 0.0;
	for (int i = 0; i < oct; i++) {
		float freq = pow(2.0, float(i));
		float amp = pow(per, float(oct - i));
		t += irnd(float2(p.x / freq, p.y / freq)) * amp;
	}
	return t;
}

// �V�[�����X�m�C�Y����
float snoise(float2 p, float2 q, float2 r) {
	return noise(float2(p.x, p.y)) * q.x * q.y +
		noise(float2(p.x, p.y + r.y)) * q.x * (1.0 - q.y) +
		noise(float2(p.x + r.x, p.y)) * (1.0 - q.x) * q.y +
		noise(float2(p.x + r.x, p.y + r.y)) * (1.0 - q.x) * (1.0 - q.y);
}



VSOutput main(VSInput input)
{
	////�X�L�j���O�v�Z
	//SkinOutput skinned = ComputeSkin(input);
	////�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	//float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));

	//float4 resultPos = skinned.pos; //�X�L�j���O��̍��W��n��
	//float4 wpos = mul(world, resultPos); //���[���h�ϊ�

	////�s�N�Z���V�F�[�_�[�ɓn���l
	//VSOutput output;
	////�s��ɂ����W�ϊ�
	//output.svpos = mul(mul(Light_viewproj, world), skinned.pos);
	//output.svpos2 = mul(mul(viewproj2, world), skinned.pos);
	//output.worldpos = wpos;
	////���[���h�@�������̃X�e�[�W�ɓn��
	//output.normal = wnormal.xyz;
	////���͒l�����̂܂܎��̃X�e�[�W�ɓn��
	//output.uv = input.uv;
	//output.InstanceID = 0;

	//return output;


	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(input.normal, 0)));

	uint tTime = uint(Time[0] * 2.0f) % 30000;
	float2 t = float2(input.pos.x + input.pos.y + tTime / 3000.0f, input.pos.z + input.pos.y + tTime / 3000.0f) * 1000.0f;
	float nn = noise(t);
	float power = 1.0f - input.pos.y / 4.0f;
	power = min(power, 1.0f);
	input.pos.x = input.pos.x + (nn * input.normal.x * power * 0.3f);
	input.pos.z = input.pos.z + (nn * input.normal.z * power * 0.3f);

	//float4 resultPos = skinned.pos; //�X�L�j���O��̍��W��n��
	float4 wpos = mul(world, input.pos); //���[���h�ϊ�

	//�s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	//�s��ɂ����W�ϊ�
	output.svpos = mul(mul(Light_viewproj, world), input.pos);
	output.svpos2 = mul(mul(viewproj2, world), input.pos);
	output.worldpos = wpos;
	//���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;
	//���͒l�����̂܂܎��̃X�e�[�W�ɓn��
	output.uv = input.uv;
	output.InstanceID = 0;

	return output;
}