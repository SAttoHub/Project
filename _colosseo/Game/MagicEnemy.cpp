#include "MagicEnemy.h"

MagicEnemy::MagicEnemy()
{
	Enemy::Initialize();
	m_Name = "ñÇèpégÇ¢";
	m_JobName = "ñÇèpét";

	m_HP = 2;
	m_MaxHP = m_HP;

	m_Attack_Datas.emplace_back("ñÇñ@ - 1", 3, 1, 0, Abnormality::None, 0);
	m_Attack_Datas.emplace_back("ñÇñ@ - 2", 5, 3, 1, Abnormality::None, 0);

	m_Attack_Datas[0].SetAttackRange(3, false);
	m_Attack_Datas[1].SetAttackRange(1, false);

	m_StartUp.m_MoveCost = 1;

	m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn_Magic8.png");
}

MagicEnemy::~MagicEnemy()
{
	Enemy::Finalize();
}
