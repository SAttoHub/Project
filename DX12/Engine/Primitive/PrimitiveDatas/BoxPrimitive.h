#pragma once
#include <DirectXMath.h>

// ���v���~�e�B�u
class BoxPrimitive {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Box {
		XMFLOAT3 pos1; //�l�̏��������_
		XMFLOAT3 pos2; //�l�̑傫�����_
		XMFLOAT4 color;
		bool Draw3D; //3D��ԏ�ɕ`�悷�邩
	};
	Box Data;
	bool Active;
};