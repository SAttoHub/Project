#include "FBX.hlsli"

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
	uint InstanceID : SV_InstanceID;
};
//�X�L�j���O��̒��_�E�@��������
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

//�{�[���̍ő吔
static const int MAX_BONES = 61;

cbuffer skinning:register(b3)//�{�[���̃X�L�j���O�s�񂪓���
{
	matrix matSkinning[MAX_BONES];
};

//�X�L�j���O�v�Z
SkinOutput ComputeSkin(VSInput input)
{
	//�[���N���A
	SkinOutput output = (SkinOutput)0;

	uint iBone; //�v�Z����{�[���ԍ�
	float weight; //�{�[���E�F�C�g(�d��)
	matrix m; //�X�L�j���O�s��

	//�{�[��0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��0
	iBone = input.boneIndicesB.x;
	weight = input.boneWeightsB.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��1
	iBone = input.boneIndicesB.y;
	weight = input.boneWeightsB.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��2
	iBone = input.boneIndicesB.z;
	weight = input.boneWeightsB.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��3
	iBone = input.boneIndicesB.w;
	weight = input.boneWeightsB.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	return output;
}

VSOutput main(VSInput input)
{
	uint index = input.InstanceID;

	//�X�L�j���O�v�Z
	//SkinOutput skinned = ComputeSkin(input);
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(data[index].world, float4(input.normal, 0)));

	//float4 resultPos = skinned.pos; //�X�L�j���O��̍��W��n��
	float4 wpos = mul(data[index].world, input.pos); //���[���h�ϊ�

	//�s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	//�s��ɂ����W�ϊ�
	output.svpos = mul(mul(viewproj, data[index].world), input.pos);
	output.svpos2 = mul(mul(viewproj2, data[index].world), input.pos);
	output.worldpos = wpos;
	//���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;
	output.InstanceID = index;
	output.uv = input.uv;
	/*if (data[index].uv.x <= -1.0f) {
		return output;
	}*/
	//���͒l�����̂܂܎��̃X�e�[�W�ɓn��
	/*if (input.uv.x <= 0.1f && input.uv.y <= 0.1f) {
		output.uv = float2(data[index].uv.x, data[index].uv.y);
	}
	else if (input.uv.x <= 0.1f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.x, data[index].uv.w);
	}
	else if (input.uv.x >= 0.9f && input.uv.y <= 0.1f) {
		output.uv = float2(data[index].uv.z, data[index].uv.y);
	}
	else if (input.uv.x >= 0.9f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.z, data[index].uv.w);
	}*/

	return output;
}

// �ϋq
VSOutput AudMain(VSInput input)
{
	uint index = input.InstanceID;

	//�X�L�j���O�v�Z
	//SkinOutput skinned = ComputeSkin(input);
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(data[index].world, float4(input.normal, 0)));

	//float4 resultPos = skinned.pos; //�X�L�j���O��̍��W��n��
	float4 wpos = mul(data[index].world, input.pos); //���[���h�ϊ�

	//�s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	//�s��ɂ����W�ϊ�
	output.svpos = mul(mul(viewproj, data[index].world), input.pos);
	output.svpos2 = mul(mul(viewproj2, data[index].world), input.pos);
	output.worldpos = wpos;
	//���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;
	output.InstanceID = index;
	output.uv = input.uv;
	if (data[index].uv.x <= -1.0f) {
		return output;
	}
	//���͒l�����̂܂܎��̃X�e�[�W�ɓn��
	if (input.uv.x <= 0.1f && input.uv.y <= 0.1f) {
		output.uv  = float2(data[index].uv.x, data[index].uv.y);
	}
	else if(input.uv.x <= 0.1f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.x, data[index].uv.w);
	}
	else if (input.uv.x >= 0.9f && input.uv.y <= 0.1f) {
		output.uv = float2(data[index].uv.z, data[index].uv.y);
	}
	else if (input.uv.x >= 0.9f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.z, data[index].uv.w);
	}

	return output;
}