#include "NoviceEnemy.h"

NoviceEnemy::NoviceEnemy()
{
	Enemy::Initialize();
	m_Name = "�����m";
	m_JobName = "���m";

	m_Attack_Datas.emplace_back("�U�� - 1", 1, 1, 0, Abnormality::None, 0);
	m_Attack_Datas.emplace_back("�U�� - 2", 3, 3, 1, Abnormality::None, 0);
}

NoviceEnemy::~NoviceEnemy()
{
	Enemy::Finalize();
}

//std::vector<AllResult> NoviceEnemy::AttackAct(int Index, XMINT2 Pos)
//{
//	// �U�������𔻒�
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
