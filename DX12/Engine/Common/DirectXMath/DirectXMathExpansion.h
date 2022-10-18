#pragma once
#include "DirectXMathOperators.h"
#include "../SDifines.h"
//��_�Ԃ̋��������߂�
static float Distance2D(const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2) {
	return sqrtf(powf(v1.x - v2.x, 2.0f) + powf(v1.y - v2.y, 2.0f));
}
//��_�Ԃ̋��������߂�
static float Distance3D(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) {
	return sqrtf(powf(v1.x - v2.x, 2.0f) + powf(v1.y - v2.y, 2.0f) + powf(v1.z - v2.z, 2.0f));
}

//���ς����߂�
static float Dot2D(const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
//���ς����߂�
static float Dot3D(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//�O�ς����߂�
static float Cross2D(const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2) {
	return v1.x * v2.y - v1.y * v2.x;
}
//�O�ς����߂�
static DirectX::XMFLOAT3 Cross3D(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) {
	return DirectX::XMFLOAT3(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

//�x�N�g���̒��������߂�
static float Length2D(const DirectX::XMFLOAT2 &v2) {
	return sqrtf(Dot2D(v2, v2));
}
//�x�N�g���̒��������߂�
static float Length3D(const DirectX::XMFLOAT3 &v2) {
	return sqrtf(Dot3D(v2, v2));
}

//�p�x�����߂�
static float CalAngle3D(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) {
	return Dot3D(v1, v2) / (Length3D(v1) * Length3D(v2));
}
static float CalAngle2D(const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2) {
	return Dot2D(v1, v2) / (Length2D(v1) * Length2D(v2));
}

static float CalcAngle(const DirectX::XMFLOAT2 &vectorA,
	const DirectX::XMFLOAT2 &vectorB)
{
	double dLengthAxLengthB = Length2D(vectorA) * Length2D(vectorB);

	double dCosAlpha = Dot2D(vectorA, vectorB) / dLengthAxLengthB;
	if (dCosAlpha > 1.0)
	{
		dCosAlpha = 1.0;
	}
	if (dCosAlpha < -1.0)
	{
		dCosAlpha = -1.0;
	}
	dCosAlpha = acos(dCosAlpha);

	return dCosAlpha = dCosAlpha * 180.0f / M_PI_F;
}


//�x�N�g���𐳋K������
static DirectX::XMFLOAT2 Normalize2D(const DirectX::XMFLOAT2 &v2) {
	float len = Length2D(v2);
	DirectX::XMFLOAT2 result = v2;
	result = result / len;
	return result;
}
//�x�N�g���𐳋K������
static DirectX::XMFLOAT3 Normalize3D(const DirectX::XMFLOAT3 &v2) {
	float len = Length3D(v2);
	DirectX::XMFLOAT3 result = v2;
	result = result / len;
	return result;
}

/// <summary>
/// �s��̗v�f��Ԃ�
/// </summary>
/// <param name="i">�s��̍s</param>
/// <param name="j">�s��̗�</param>
/// <param name="mat">�Ԃ������l���������s��</param>
/// <returns></returns>
static float matGet(int i, int j, const DirectX::XMMATRIX &mat) {
	return mat.r[i].m128_f32[j];
}

/// <summary>
/// �s����g�����x�N�g���̕ϊ�
/// </summary>
/// <param name="vec">�ϊ��������s�������x�N�g��</param>
/// <param name="mat"�ϊ������Ɏg�p����s��></param>
/// <returns></returns>
static DirectX::XMVECTOR VTransform(const DirectX::XMVECTOR &vec, const DirectX::XMMATRIX &mat)
{
	DirectX::XMVECTOR Result;
	Result.m128_f32[0] = vec.m128_f32[0] * matGet(0, 0, mat) + vec.m128_f32[1] * matGet(1, 0, mat) + vec.m128_f32[2] * matGet(2, 0, mat) + matGet(3, 0, mat);
	Result.m128_f32[1] = vec.m128_f32[0] * matGet(0, 1, mat) + vec.m128_f32[1] * matGet(1, 1, mat) + vec.m128_f32[2] * matGet(2, 1, mat) + matGet(3, 1, mat);
	Result.m128_f32[2] = vec.m128_f32[0] * matGet(0, 2, mat) + vec.m128_f32[1] * matGet(1, 2, mat) + vec.m128_f32[2] * matGet(2, 2, mat) + matGet(3, 2, mat);
	return Result;
}

static DirectX::XMFLOAT2 FtoI(DirectX::XMINT2 v)
{
	return DirectX::XMFLOAT2(static_cast<float>(v.x), static_cast<float>(v.y));
}
static DirectX::XMFLOAT3 FtoI(DirectX::XMINT3 v)
{
	return DirectX::XMFLOAT3(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z));
}
static DirectX::XMFLOAT4 FtoI(DirectX::XMINT4 v)
{
	return DirectX::XMFLOAT4(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z), static_cast<float>(v.w));
}
static DirectX::XMINT2 ItoF(DirectX::XMFLOAT2 v)
{
	return DirectX::XMINT2(static_cast<int32_t>(v.x), static_cast<int32_t>(v.y));
}
static DirectX::XMINT3 ItoF(DirectX::XMFLOAT3 v)
{
	return DirectX::XMINT3(static_cast<int32_t>(v.x), static_cast<int32_t>(v.y), static_cast<int32_t>(v.z));
}
static DirectX::XMINT4 ItoF(DirectX::XMFLOAT4 v)
{
	return DirectX::XMINT4(static_cast<int32_t>(v.x), static_cast<int32_t>(v.y), static_cast<int32_t>(v.z), static_cast<int32_t>(v.w));
}