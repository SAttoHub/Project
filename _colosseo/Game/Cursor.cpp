#include "Cursor.h"
#include "..\Input.h"

Cursor::~Cursor()
{
	if (m_Collider) {
		GCM::Instance()->RemoveCollider(m_Collider);
		//delete collider;
	}
	/*if (col) {
		delete col;
	}*/
}

void Cursor::SetCollider()
{
	m_Collider = new GamePointCollider(&m_MousePos, "Mouse");
	m_Collider->SetBasisPosition(&m_MousePos);
	GCM::Instance()->AddCollider(m_Collider);
}

void Cursor::ColliderUpdate()
{
	if (m_Collider) {
		m_Collider->Update();
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
	m_CusorImg = TexManager::LoadTexture("Resource/image/Cursor.png");
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
		m_CusorImg);
}
