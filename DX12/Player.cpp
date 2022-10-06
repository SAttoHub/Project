#include "Player.h"

Player::Player()
{
	m_MapPos = { 0,0 };
	m_HP = 100;

	int modelData = LoadModelOBJ("Charactor", "mob");
	model = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	model->rotation.y = 90.0f;
	model->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f);
	model->material.texNumber = TexManager::GetColor(XMFLOAT4(0, 255, 255, 255));
}

Player::~Player()
{
}

void Player::SetMap(Map *Map)
{
	pMap = Map;
}

void Player::Update()
{
	model->position = pMap->ChangePos(m_MapPos);
}

void Player::Draw()
{
	Drawobject3d(model);
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(10, 400), 32, XMFLOAT4(1, 1, 1, 1),
		"PlayerHP : %d", m_HP);
}
