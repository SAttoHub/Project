#pragma once
#include "DirectXMathExpansion.h"
#include "Camera.h"

class ShadowMapLight {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static XMFLOAT3 Pos;
	static XMFLOAT3 Target;
	static XMFLOAT3 Up;
	static XMMATRIX matView;
	static XMMATRIX matViewProjection;
	static XMMATRIX matViewProjectionInverse;

	static void SetLightPos(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up);
};

