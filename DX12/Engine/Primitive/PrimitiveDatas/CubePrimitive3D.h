#pragma once
#include <DirectXMath.h>

// �����̃v���~�e�B�u
class CubePrimitive3D {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Cube {
		XMFLOAT3 pos1; //�l�̏��������_
		XMFLOAT3 pos2; //�l�̑傫�����_
		XMFLOAT3 Scale = XMFLOAT3(1, 1, 1);
		XMFLOAT3 Rotate = XMFLOAT3(0, 0, 0);
		XMFLOAT4 color;
		bool Lighting;
	};
	Cube Data;
	//XMFLOAT3 Rotate = XMFLOAT3(0, 0, 0);
	bool Active;
};