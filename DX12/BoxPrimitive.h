#pragma once
#include <DirectXMath.h>

// 箱プリミティブ
class BoxPrimitive {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct Box {
		XMFLOAT3 pos1; //値の小さい頂点
		XMFLOAT3 pos2; //値の大きい頂点
		XMFLOAT4 color;
		bool Draw3D; //3D空間上に描画するか
	};
	Box Data;
	bool Active;
};