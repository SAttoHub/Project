#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Point
{
	DirectX::XMFLOAT2 pos = {};
};

struct Box
{
	//座標
	DirectX::XMFLOAT2 pos1 = {};
	DirectX::XMFLOAT2 pos2 = {};
};

struct Circle
{
	// 中心座標
	DirectX::XMFLOAT2 center = {};
	// 半径
	float radius = 1.0f;
};

struct Sphere
{
	// 中心座標
	DirectX::XMVECTOR center = {};
	// 半径
	float radius = 1.0f;
};

struct Plane
{
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0 };
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

class Triangle
{
public:
	// 頂点座標3つ
	DirectX::XMVECTOR	p0;
	DirectX::XMVECTOR	p1;
	DirectX::XMVECTOR	p2;
	// 法線ベクトル
	DirectX::XMVECTOR	normal;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal() {
		XMVECTOR p0_p1 = p1 - p0;
		XMVECTOR p0_p2 = p2 - p0;

		// 外積により、2辺に垂直なベクトルを算出する
		normal = XMVector3Cross(p0_p1, p0_p2);
		normal = XMVector3Normalize(normal);
	};
};

struct Ray
{
	// 始点座標
	DirectX::XMVECTOR	start = { 0,0,0,1 };
	// 方向
	DirectX::XMVECTOR	dir = { 1,0,0,0 };
};