#include "MagicBossEnemy.h"

MagicBossEnemy::MagicBossEnemy()
{
	Enemy::Initialize();
	m_Name = "���p�g���E��";
	m_JobName = "�{�X���p�t";
	m_isBoss = true;
	m_pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.6f;

	m_HP = 10;
	m_MaxHP = m_HP;

	/// <param name="Name"> �Z�� </param>
	/// <param name="Damage"> �_���[�W </param>
	/// <param name="Cost"> �g�p�R�X�g </param>
	/// <param name="KnockBackPower"> �m�b�N�o�b�N�� </param>
	/// <param name="Abn"> �^�����Ԉُ� </param>
	/// <param name="AbnTurn"> ��Ԉُ�̃^�[�� </param>
	m_Attack_Datas.emplace_back("���@ - 1", 6, 1, 0, Abnormality::None, 0);
	m_Attack_Datas.emplace_back("���@ - 2", 15, 3, 0, Abnormality::None, 0);

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
