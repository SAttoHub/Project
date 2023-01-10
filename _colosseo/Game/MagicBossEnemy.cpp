#include "MagicBossEnemy.h"

MagicBossEnemy::MagicBossEnemy()
{
	Enemy::Initialize();
	m_Name = "魔術使い・強";
	m_JobName = "ボス魔術師";
	m_isBoss = true;
	m_pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.6f;

	m_HP = 10;
	m_MaxHP = m_HP;

	/// <param name="Name"> 技名 </param>
	/// <param name="Damage"> ダメージ </param>
	/// <param name="Cost"> 使用コスト </param>
	/// <param name="KnockBackPower"> ノックバック量 </param>
	/// <param name="Abn"> 与える状態異常 </param>
	/// <param name="AbnTurn"> 状態異常のターン </param>
	m_Attack_Datas.emplace_back("魔法 - 1", 6, 1, 0, Abnormality::None, 0);
	m_Attack_Datas.emplace_back("魔法 - 2", 15, 3, 0, Abnormality::None, 0);

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

MagicBossEnemy::~MagicBossEnemy()
{
	Enemy::Finalize();
}
