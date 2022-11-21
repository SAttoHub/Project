#include "GameCircleCollider.h"

void GameCircleCollider::Update()
{
	Center = *Position;
	// ‰~‚Ìƒƒ“ƒo•Ï”‚ğXV
	Circle::center = Center;
	Circle::radius = Radius;
}

void GameCircleCollider::Draw()
{
	// ‰~‚ÌDrawì‚Á‚Ä‚Ü‚¹‚ñ
}
