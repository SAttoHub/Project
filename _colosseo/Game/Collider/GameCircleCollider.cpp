#include "GameCircleCollider.h"

void GameCircleCollider::Update()
{
	Center = *Position;
	// �~�̃����o�ϐ����X�V
	Circle::center = Center;
	Circle::radius = Radius;
}

void GameCircleCollider::Draw()
{
	// �~��Draw����Ă܂���
}
