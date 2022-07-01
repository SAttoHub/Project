#include "ShadowMapLight.h"

DirectX::XMFLOAT3 ShadowMapLight::Pos{ 0,0,0 };
DirectX::XMFLOAT3 ShadowMapLight::Target{ 0,0,0 };
DirectX::XMFLOAT3 ShadowMapLight::Up{ 0,0,0 };
DirectX::XMMATRIX ShadowMapLight::matView;
DirectX::XMMATRIX ShadowMapLight::matViewProjection;
DirectX::XMMATRIX ShadowMapLight::matViewProjectionInverse;
DirectX::XMMATRIX ShadowMapLight::matViewProjectionInverseCamera;

void ShadowMapLight::SetLightPos(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up)
{

	XMMATRIX matProj = XMMatrixOrthographicLH(
		(float)WINDOW_WIDTH , (float)WINDOW_HEIGHT, //アスペクト比
		0.1f, 1000.0f //前端、奥端
	);

	Pos = pos;
	Target = target;
	Up = up;
	matView = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&Pos),
		DirectX::XMLoadFloat3(&Target),
		DirectX::XMLoadFloat3(&Up));

	matViewProjection = matView * matProj;

	//
	matViewProjectionInverseCamera = XMMatrixInverse(nullptr, Camera::matViewProjection);

	int aaa = 0;
	aaa = 1;
}
