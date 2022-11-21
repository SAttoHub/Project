#include "GameBoxCollider.h"

void GameBoxCollider::Update()
{
	Box::pos1 = Pos1 + *Position;
	Box::pos2 = Pos2 + *Position;
}

void GameBoxCollider::Draw()
{
	DrawBox(Box::pos1, Box::pos2, ColorConvert(DirectX::XMFLOAT4(0, 255, 255, 255)));
}

inline void GameBoxCollider::SetBoxSize(XMFLOAT2 pos1, XMFLOAT2 pos2)
{
	this->Pos1 = pos1;
	this->Pos2 = pos2;
}
