#include "FBX.hlsli"

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