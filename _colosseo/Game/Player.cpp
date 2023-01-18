#include "Player.h"
#include "GameCamera.h"

Player::Player()
{
	m_MapPos = { 6,6 };
	m_HP = 100;
	m_MaxHP = 100;

	int modelData = LoadModelOBJ("Charactor", "mob");
	m_pModel = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(-1000, -1000, -1000), FBXSHADER);
	m_pModel->rotation.y = 90.0f;
	m_pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.3f;
	m_pModel->material.texNumber = TexManager::GetColor(XMFLOAT4(0, 255, 255, 255));
	m_pModel->isBillboard = true;
	m_pModel->UseShadow = true;
	m_pModel->UseDOF = true;

	m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/Chara1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/Chara2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/Chara3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/Chara4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/Chara5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/Chara6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/Chara7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/Chara8.png");
	m_pModel->alwaysUpdate = true;
}

Player::~Player()
{
	DirectX3dObject::DeleteObject(m_pModel);
}

void Player::SetMap(Map *Map)
{
	m_pMap = Map;
}

void Player::Update()
{
	WaitUpdate();

	if (Input::isKeyTrigger(DIK_J)) {
		int hoge = int(m_NowDir) + 1;
		if (hoge >= int(Chara_Dir::MAX)) {
			hoge = hoge - int(Chara_Dir::MAX);
		}
		m_NowDir = Chara_Dir(hoge);
	}

	ReactionUpdate();
	m_pModel->position = GetWorldPos() + m_ReactionOffset;
	SetDirFromCamera();
	//m_pModel->material.texNumber = m_Image[(int)m_DirFromCamera];
	//m_pModel->material.texNumber = m_Image[(int)GetDrawDir()];
}

void Player::Draw()
{
	m_pModel->position = GetWorldPos() + m_ReactionOffset;
	m_pModel->material.texNumber = m_Image[(int)GetDrawDir()];
	Drawobject3d(m_pModel);
	/*DrawStrings::Instance()->DrawFormatString(XMFLOAT2(10, 400), 32, XMFLOAT4(1, 1, 1, 1),
		"PlayerHP : %d", m_HP);*/
}

void Player::Reset()
{
	m_MapPos = { 6,6 };
	m_HP = 100;
	m_ReactionOffset = XMFLOAT3();
	m_NowReaction = Reactions::Reac_None;
}
