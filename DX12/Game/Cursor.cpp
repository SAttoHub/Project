#include "Cursor.h"
#include "..\Input.h"

Cursor::~Cursor()
{
	if (collider) {
		GCM::Instance()->RemoveCollider(collider);
		//delete collider;
	}
	/*if (col) {
		delete col;
	}*/
}

void Cursor::SetCollider(GameBaseCollider *collider)
{
	collider->SetBasisPosition(&m_MousePos);
	this->collider = collider;
	GCM::Instance()->AddCollider(collider);
}

void Cursor::ColliderUpdate()
{
	if (collider) {
		collider->Update();
	}
}

Cursor::Cursor()
{
	m_MousePos = {};
}

void Cursor::Initialize()
{
	m_MousePos = Input::GetMousePos();
	col = new GamePointCollider(&m_MousePos, "Mouse");
	SetCollider(col);
}

void Cursor::Update()
{
	m_MousePos = Input::GetMousePos();
	ColliderUpdate();
}

void Cursor::Draw()
{
	DrawGraph(m_MousePos - XMFLOAT2(6, 6),
		m_MousePos + XMFLOAT2(6, 6),
		TexManager::GetColor(ColorConvert2(XMFLOAT4(0, 0, 1, 1))));
}
