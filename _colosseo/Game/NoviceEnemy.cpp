#include "NoviceEnemy.h"

NoviceEnemy::NoviceEnemy()
{
	Enemy::Initialize();
	m_Name = "åïì¨ém";
	m_JobName = "åïém";

	m_HP = 2;
	m_MaxHP = m_HP;

	m_Attack_Datas.emplace_back("çUåÇ - 1", 1, 1, 0, Abnormality::None, 0);
	m_Attack_Datas.emplace_back("çUåÇ - 2", 1, 3, 1, Abnormality::None, 0);

	m_Attack_Datas[0].SetAttackRange(1, false);
	m_Attack_Datas[1].SetAttackRange(1, false);

	m_StartUp.m_MoveCost = 3;

	m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn8.png");
}

NoviceEnemy::~NoviceEnemy()
{
	Enemy::Finalize();
}

//std::vector<AllResult> NoviceEnemy::AttackAct(int Index, XMINT2 Pos)
//{
//	// çUåÇï˚å¸ÇîªíË
//	XMINT2 Hoge = m_MapPos - Pos;
//
//	Enemy_Attack_Data AtkData = m_Attack_Datas[Index];
//
//	KnockBack KnB(AtkData.m_KnockBackPower, 
//		Hoge.y > 0 ? true : false,
//		Hoge.y < 0 ? true : false,
//		Hoge.x < 0 ? true : false,
//		Hoge.x > 0 ? true : false);
//	return ActionManager::Action(Pos, AtkData.m_Damage, KnB, AtkData.m_Abn, AtkData.m_AbnTurn);
//}
