#include "FBX.hlsli"

[maxvertexcount(4)]
void main(
	triangle VSOutput input[3] : SV_POSITION,
	inout LineStream< GSOutput > output
)
{
    GSOutput aaa;

	/* ���C�g�ւ̕����x�N�g��
	float3 lightv = cameraPos - input[0].worldpos.xyz;
	float3 d = length(lightv);
	lightv = normalize(lightv);

	 ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input[0].worldpos.xyz);
	 ����x
	const float shininess = 4.0f;


	 ���C�g�Ɍ������x�N�g���Ɩ@���̓���
	float3 dotlightnormal = dot(lightv, input[0].normal);
	 ���ˌ��x�N�g��
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input[0].normal);
	 �g�U���ˌ�
	float3 diffuse = dotlightnormal * m_diffuse;
	 ���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	�e�����}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳����
	float3 result = (diffuse + specular);
	if (result.r < 0.0f) result.r = 0.0f;
	if (result.g < 0.0f) result.g = 0.0f;
	if (result.b < 0.0f) result.b = 0.0f;*/

	//if (result.r != 0.0f || result.g != 0.0f || result.b != 0.0f) {
		for (uint i = 0; i < 3; i++) {
			aaa.svpos = input[i].svpos;
			aaa.color = float4(0, 0, 0, 1);
			output.Append(aaa);
		}
		aaa.svpos = input[0].svpos;
		aaa.color = float4(0, 0, 0, 1);
		output.Append(aaa);
	//}
}