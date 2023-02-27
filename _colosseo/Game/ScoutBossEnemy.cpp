#include "ScoutBossEnemy.h"

ScoutBossEnemy::ScoutBossEnemy()
{
	Enemy::Initialize();
	m_Name = "åyã∆émÅEã≠";
	m_JobName = "É{ÉXêÀåÛ";
	m_isBoss = true;
	m_pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.6f;

	m_HP = 1;
	m_MaxHP = m_HP;

	m_Attack_Datas.emplace_back("çUåÇ - 1", 2, 1, 0, Abnormality::None, 0);

	m_Attack_Datas[0].SetAttackRange(1, true);

	m_StartUp.m_MoveCost = 5;

	m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Scout8.png");
}

ScoutBossEnemy::~ScoutBossEnemy()
{
	Enemy::Finalize();
}
