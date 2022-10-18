#pragma once
#include <DirectXMath.h>

// 線プリミティブ
class LinePrimitive {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Line {
		XMFLOAT3 pos1;
		XMFLOAT3 pos2;
		XMFLOAT4 color;
	};
	Line Data;
	bool Active;
};