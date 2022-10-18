#include "GameRayCollider.h"

void GameRayCollider::Update()
{
	Ray::start = ray_.start;
	Ray::dir = ray_.dir;
}

void GameRayCollider::Draw()
{
}
