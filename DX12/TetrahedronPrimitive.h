#pragma once
#include <DirectXMath.h>

// 正四面体プリミティブ
class TetrahedronPrimitive {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Tetrahedron {
		XMFLOAT3 pos1; //頂点1
		XMFLOAT3 pos2; //頂点2
		XMFLOAT3 pos3; //頂点3
		XMFLOAT3 pos4; //頂点4
		XMFLOAT4 color;
		bool Lighting;
	};
	Tetrahedron Data;
	bool Active;
};