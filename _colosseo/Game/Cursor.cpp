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

void Cursor::SetCollider()
{
	collider = new GamePointCollider(&m_MousePos, "Mouse");
	collider->SetBasisPosition(&m_MousePos);
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
	SetCollider();
	CusorImg = TexManager::LoadTexture("Resource/image/Cursor.png");
}

void Cursor::Update()
{
	m_MousePos = Input::GetMousePos();
	ColliderUpdate();
}

void Cursor::Draw()
{
	DrawGraph(m_MousePos,
		m_MousePos + XMFLOAT2(24, 24),
		CusorImg);
}
