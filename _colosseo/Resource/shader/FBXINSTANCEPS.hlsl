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
cbuffer time : register(b7)
{
	uint Time;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

//float smoothstepf(float edge0, float edge1, float x) {
//	float t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
//	return t * t * (3 - 2 * t);
//}
//
//float smoothstep(float edge0, float edge1, float3 f) {
//	float3 res = { 0,0,0 };
//	res.x = smoothstepf(edge0, edge1, f.x);
//	res.y = smoothstepf(edge0, edge1, f.y);
//	res.z = smoothstepf(edge0, edge1, f.z);
//	return res;
//}

float4 main(VSOutput input) : SV_TARGET
{
	////�e�N�X�`���}�b�s���O
	//float4 texcolor = tex.Sample(smp, input.uv);
	////Lambert����
	//float3 light = normalize(float3(1,-1,1)); //�E�����@�����̃��C�g
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	////�A�e�ƃe�N�X�`���̐F������
	//return shadecolor * texcolor;
	// �e�N�X�`���}�b�s���O
	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;
	
	if (data[index].DrawFlag == false) {
		discard;
	}

	float4 texcolor = tex.Sample(smp, input.uv);
	if (InColor.w == -1.0f) {
		return texcolor * float4(InColor.r, InColor.g, InColor.b, 1.0f);
	}

	// ����x
	const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	shadecolor = float4(0, 0, 0, 1);
	float3 dotlightnormal = { 0, 0, 0 };
	float3 reflect = { 0, 0, 0 };
	float3 diffuse = { 0, 0, 0 };
	float3 specular = { 0, 0, 0 };
	float3 lightpos = { 0, 0, 0 };
	float3 lightv = { 0, 0, 0 };
	float d = 0;
	float atten = 0;
	float cos = 0;
	float angleatten = 0;
	float3 casterv = { 0, 0, 0 };

	//�g�D�[��
	float _DiffuseThreshold = 0.1f;
	float _DiffuseBlur = 0.05f;
	float _SpecularThreshold = 0.9f;
	float _SpecularBlur = 0.05f;
	//�A���r�G���g���Â������̂Œl�𒼐ړ����


	// ���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// ���ˌ��x�N�g��
			reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//�g�D�[��
			dotlightnormal = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, dotlightnormal);
			// �g�U���ˌ�
			diffuse = dotlightnormal * m_diffuse;
			//�g�D�[��2
			float3 ttt = pow(saturate(dot(reflect, eyedir)), shininess);
			ttt = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, ttt);
			// ���ʔ��ˌ�
			specular = ttt * m_specular;
			//�e�����}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳����
			float3 result = (diffuse + specular) * dirLights[i].lightcolor;
			result.r = max(result.r, 0.0f);
			result.g = max(result.g, 0.0f);
			result.b = max(result.b, 0.0f);
			// �S�ĉ��Z����
			shadecolor.rgb += result;
		}
	}

	// �_����
	for (i = 0; i < POINTLIGHT_NUM; i++) {
		if (pointLights[i].active) {
			// ���C�g�ւ̕����x�N�g��
			lightv = pointLights[i].lightpos - input.worldpos.xyz;
			d = length(lightv);
			lightv = normalize(lightv);

			// ���������W��
			atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			dotlightnormal = dot(lightv, input.normal);
			// ���ˌ��x�N�g��
			reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�D�[��
			dotlightnormal = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, dotlightnormal);
			// �g�U���ˌ�
			diffuse = dotlightnormal * m_diffuse;
			//�g�D�[��2
			float3 ttt = pow(saturate(dot(reflect, eyedir)), shininess);
			ttt = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, ttt);
			// ���ʔ��ˌ�
			specular = ttt * m_specular;
			//�e�����}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳����
			float3 result = atten * (diffuse + specular) * pointLights[i].lightcolor;
			result.r = max(result.r, 0.0f);
			result.g = max(result.g, 0.0f);
			result.b = max(result.b, 0.0f);
			// �S�ĉ��Z����
			shadecolor.rgb += result;
		}
	}

	// �X�|�b�g���C�g
	for (i = 0; i < SPOTLIGHT_NUM; i++) {
		if (spotLights[i].active) {
			// ���C�g�ւ̕����x�N�g��
			lightv = spotLights[i].lightpos - input.worldpos.xyz;
			d = length(lightv);
			lightv = normalize(lightv);

			// ���������W��
			atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));

			// �p�x����
			cos = dot(lightv, spotLights[i].lightv);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			// �p�x��������Z
			atten *= angleatten;

			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			dotlightnormal = dot(lightv, input.normal);
			// ���ˌ��x�N�g��
			reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�D�[��
			dotlightnormal = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, dotlightnormal);
			// �g�U���ˌ�
			diffuse = dotlightnormal * m_diffuse;
			//�g�D�[��2
			float3 ttt = pow(saturate(dot(reflect, eyedir)), shininess);
			ttt = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, ttt);
			// ���ʔ��ˌ�
			specular = ttt * m_specular;
			//�e�����}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳����
			float3 result = atten * (diffuse + specular) * spotLights[i].lightcolor;
			result.r = max(result.r, 0.0f);
			result.g = max(result.g, 0.0f);
			result.b = max(result.b, 0.0f);
			// �S�ĉ��Z����
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}

	// �ۉe
	for (i = 0; i < CIRCLESHADOW_NUM; i++) {
		if (circleShadows[i].active) {
			// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
			casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			// ���������ł̋���
			d = dot(casterv, circleShadows[i].dir);

			// ���������W��
			atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			// �������}�C�i�X�Ȃ�0�ɂ���
			atten *= step(0, d);

			// ���C�g�̍��W
			lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//  �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
			lightv = normalize(lightpos - input.worldpos.xyz);
			// �p�x����
			cos = dot(lightv, circleShadows[i].dir);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// �p�x��������Z
			atten *= angleatten;

			// �S�Č��Z����
			shadecolor.rgb -= atten;
		}
	}

	if (shadecolor.r > 0.6f) shadecolor.r = 0.9f;
	//if (shadecolor.r <= 0.6f) shadecolor.r = 0.6f;
	if (shadecolor.g > 0.6f) shadecolor.g = 0.9f;
	//if (shadecolor.g <= 0.6f) shadecolor.g = 0.6f;
	if (shadecolor.b > 0.6f) shadecolor.b = 0.9f;
	//if (shadecolor.b <= 0.6f) shadecolor.b = 0.6f;
	shadecolor.r = max(shadecolor.r, 0.6f);
	shadecolor.g = max(shadecolor.g, 0.6f);
	shadecolor.b = max(shadecolor.b, 0.6f);

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 10.0f) {
		DisAlpha = dist / 10.0f;
		/*if (DisAlpha < 0.0f) {
			DisAlpha = 0.0f;
		}*/
		DisAlpha = max(DisAlpha, 0.0f);
	}
	float4 ResultColor = shadecolor * texcolor * InColor;
	ResultColor.a = ResultColor.a * DisAlpha;
	//smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);


	if (ResultColor.a <= 0) {
		discard;
	}


	// �V�F�[�f�B���O�ɂ��F�ŕ`��
	return ResultColor;
}


//----------------- Guide
float3 rgb2hsv(float3 rgb)
{
	float3 hsv;

	// RGB�̎O�̒l�ōő�̂���
	float maxValue = max(rgb.r, max(rgb.g, rgb.b));
	// RGB�̎O�̒l�ōŏ��̂���
	float minValue = min(rgb.r, min(rgb.g, rgb.b));
	// �ő�l�ƍŏ��l�̍�
	float delta = maxValue - minValue;

	// V�i���x�j
	// ��ԋ����F��V�l�ɂ���
	hsv.z = maxValue;

	// S�i�ʓx�j
	// �ő�l�ƍŏ��l�̍��𐳋K�����ċ��߂�
	if (maxValue != 0.0) {
		hsv.y = delta / maxValue;
	}
	else {
		hsv.y = 0.0;
	}

	// H�i�F���j
	// RGB�̂����ő�l�ƍŏ��l�̍����狁�߂�
	if (hsv.y > 0.0) {
		if (rgb.r == maxValue) {
			hsv.x = (rgb.g - rgb.b) / delta;
		}
		else if (rgb.g == maxValue) {
			hsv.x = 2 + (rgb.b - rgb.r) / delta;
		}
		else {
			hsv.x = 4 + (rgb.r - rgb.g) / delta;
		}
		hsv.x /= 6.0;
		if (hsv.x < 0)
		{
			hsv.x += 1.0;
		}
	}

	return hsv;
}
// HSV->RGB�ϊ�
float3 hsv2rgb(float3 hsv)
{
	float3 rgb;

	if (hsv.y == 0) {
		// S�i�ʓx�j��0�Ɠ������Ȃ�Ζ��F�������͊D�F
		rgb.r = rgb.g = rgb.b = hsv.z;
	}
	else {
		// �F��H�i�F���j�̈ʒu��S�i�ʓx�j�AV�i���x�j����RGB�l���Z�o����
		hsv.x *= 6.0;
		float i = floor(hsv.x);
		float f = hsv.x - i;
		float aa = hsv.z * (1 - hsv.y);
		float bb = hsv.z * (1 - (hsv.y * f));
		float cc = hsv.z * (1 - (hsv.y * (1 - f)));
		if (i < 1) {
			rgb.r = hsv.z;
			rgb.g = cc;
			rgb.b = aa;
		}
		else if (i < 2) {
			rgb.r = bb;
			rgb.g = hsv.z;
			rgb.b = aa;
		}
		else if (i < 3) {
			rgb.r = aa;
			rgb.g = hsv.z;
			rgb.b = cc;
		}
		else if (i < 4) {
			rgb.r = aa;
			rgb.g = bb;
			rgb.b = hsv.z;
		}
		else if (i < 5) {
			rgb.r = cc;
			rgb.g = aa;
			rgb.b = hsv.z;
		}
		else {
			rgb.r = hsv.z;
			rgb.g = aa;
			rgb.b = bb;
		}
	}
	return rgb;
}

// ����

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

float4 Guide(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;

	if (data[index].DrawFlag == false) {
		discard;
	}

	float4 texcolor = tex.Sample(smp, input.uv);
	float3 TexHsv = rgb2hsv(texcolor.xyz);
	float3 ColorHsv = rgb2hsv(InColor.xyz); 
	TexHsv.x = ColorHsv.x;
	float3 RGB = hsv2rgb(TexHsv);
	float InColW = InColor.w;
	if (InColor.w < 0.0f) {
		InColW = -InColor.w;
		if (input.worldpos.y > 0.1f) {
			discard;
		}
	}
	float4 ResultColor = float4(RGB.x, RGB.y, RGB.z, texcolor.w * InColW);

	//float2 t = float2(input.uv.x, input.uv.x) * (Time * 10.0f);

	int tTime = Time % 30000;
	float2 t = float2(input.uv.x + tTime / 3000.0f, input.uv.x + tTime / 3000.0f) * 1000.0f;

	if (input.worldpos.y > 0.1f) {
		float nn = noise(t);
		if (input.worldpos.y > nn * 4.0f) {
			discard;
		}
		ResultColor.a = 0.51f + nn / 3.0f;
	}

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 10.0f) {
		DisAlpha = dist / 10.0f;
		if (DisAlpha < 0.0f) {
			DisAlpha = 0.0f;
		}
	}

	ResultColor.a = ResultColor.a * DisAlpha;

	if (ResultColor.a == 0.0f) discard;

	// �V�F�[�f�B���O�ɂ��F�ŕ`��
	return ResultColor;
}