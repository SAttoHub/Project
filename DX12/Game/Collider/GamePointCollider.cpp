#include "GamePointCollider.h"

void GamePointCollider::Update()
{
	Point::pos = *Position;
}

void GamePointCollider::Draw()
{
	DrawBox(Point::pos - XMFLOAT2(0.5f, 0.5f), Point::pos + XMFLOAT2(0.5f, 0.5f), ColorConvert(DirectX::XMFLOAT4(0, 255, 255, 255)));
}
