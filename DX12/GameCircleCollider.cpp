#include "GameCircleCollider.h"

void GameCircleCollider::Update()
{
	Center = *Position;
	// 円のメンバ変数を更新
	Circle::center = Center;
	Circle::radius = Radius;
}

void GameCircleCollider::Draw()
{
	// 円のDraw作ってません
}
