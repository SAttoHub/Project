#include "Player.h"

Player::Player()
{
	m_MapPos = { 0,0 };
	m_HP = 100;

	int modelData = LoadModelOBJ("Charactor", "mob");
	pModel = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(-1000, -1000, -1000), FBXSHADER);
	pModel->rotation.y = 90.0f;
	pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.3f;
	pModel->material.texNumber = TexManager::GetColor(XMFLOAT4(0, 255, 255, 255));
	pModel->isBillboard = true;
	pModel->UseShadow = true;
	pModel->UseDOF = true;

	Image[0] = TexManager::LoadTexture("Resource/image/Chara/Chara1.png");
	Image[1] = TexManager::LoadTexture("Resource/image/Chara/Chara2.png");
	Image[2] = TexManager::LoadTexture("Resource/image/Chara/Chara3.png");
	Image[3] = TexManager::LoadTexture("Resource/image/Chara/Chara4.png");
	Image[4] = TexManager::LoadTexture("Resource/image/Chara/Chara5.png");
	Image[5] = TexManager::LoadTexture("Resource/image/Chara/Chara6.png");
	Image[6] = TexManager::LoadTexture("Resource/image/Chara/Chara7.png");
	Image[7] = TexManager::LoadTexture("Resource/image/Chara/Chara8.png");
	pModel->alwaysUpdate = true;
}

Player::~Player()
{
	DirectX3dObject::DeleteObject(pModel);
}

void Player::SetMap(Map *Map)
{
	pMap = Map;
}

void Player::Update()
{
	pModel->position = pMap->ChangePos(m_MapPos);

	SetDir();
	pModel->material.texNumber = Image[(int)m_Dir];
}

void Player::Draw()
{
	Drawobject3d(pModel);
	/*DrawStrings::Instance()->DrawFormatString(XMFLOAT2(10, 400), 32, XMFLOAT4(1, 1, 1, 1),
		"PlayerHP : %d", m_HP);*/
}

void Player::Reset()
{
	m_MapPos = { 0,0 };
	m_HP = 100;
}
