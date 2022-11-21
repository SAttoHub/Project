#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Point
{
	DirectX::XMFLOAT2 pos = {};
};

struct Box
{
	//���W
	DirectX::XMFLOAT2 pos1 = {};
	DirectX::XMFLOAT2 pos2 = {};
};

struct Circle
{
	// ���S���W
	DirectX::XMFLOAT2 center = {};
	// ���a
	float radius = 1.0f;
};

struct Sphere
{
	// ���S���W
	DirectX::XMVECTOR center = {};
	// ���a
	float radius = 1.0f;
};

struct Plane
{
	// �@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0 };
	// ���_(0,0,0)����̋���
	float distance = 0.0f;
};

class Triangle
{
public:
	// ���_���W3��
	DirectX::XMVECTOR	p0;
	DirectX::XMVECTOR	p1;
	DirectX::XMVECTOR	p2;
	// �@���x�N�g��
	DirectX::XMVECTOR	normal;

	/// <summary>
	/// �@���̌v�Z
	/// </summary>
	void ComputeNormal() {
		XMVECTOR p0_p1 = p1 - p0;
		XMVECTOR p0_p2 = p2 - p0;

		// �O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
		normal = XMVector3Cross(p0_p1, p0_p2);
		normal = XMVector3Normalize(normal);
	};
};

struct Ray
{
	// �n�_���W
	DirectX::XMVECTOR	start = { 0,0,0,1 };
	// ����
	DirectX::XMVECTOR	dir = { 1,0,0,0 };
};