#pragma once
#include <DirectXMath.h>

// ���l�ʑ̃v���~�e�B�u
class TetrahedronPrimitive {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Tetrahedron {
		XMFLOAT3 pos1; //���_1
		XMFLOAT3 pos2; //���_2
		XMFLOAT3 pos3; //���_3
		XMFLOAT3 pos4; //���_4
		XMFLOAT4 color;
		bool Lighting;
	};
	Tetrahedron Data;
	bool Active;
};