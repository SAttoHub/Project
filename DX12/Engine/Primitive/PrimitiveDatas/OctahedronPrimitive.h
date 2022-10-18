#pragma once
#include <DirectXMath.h>

// �����ʑ̃v���~�e�B�u
class OctahedronPrimitive {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Octahedron {
		XMFLOAT3 pos; //���S�_
		float Radius; //���a
		XMFLOAT4 color;
		bool Lighting;
	};
	Octahedron Data;
	bool Active;
};