#pragma once
#include <DirectXMath.h>

// 正八面体プリミティブ
class OctahedronPrimitive {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Octahedron {
		XMFLOAT3 pos; //中心点
		float Radius; //半径
		XMFLOAT4 color;
		bool Lighting;
	};
	Octahedron Data;
	bool Active;
};