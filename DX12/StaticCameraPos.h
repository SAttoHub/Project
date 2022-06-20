#pragma once
#include "DirectXMathExpansion.h"
#include "Singleton.h"

class StaticCameraPos : public Singleton<StaticCameraPos> {
	friend class Singleton<StaticCameraPos>;
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
public:
	XMFLOAT3 position;
	XMFLOAT3 target;
	StaticCameraPos();
	~StaticCameraPos();
};

