#include "NoviceBossEnemy.h"

NoviceBossEnemy::NoviceBossEnemy()
{
	Enemy::Initialize();
	m_Name = "�����m�E��";
	m_JobName = "�{�X���m";
	m_isBoss = true;
	m_pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.6f;

	m_HP = 20;
	m_MaxHP = m_HP;

	/// <param name="Name"> �Z�� </param>
	/// <param name="Damage"> �_���[�W </param>
	/// <param name="Cost"> �g�p�R�X�g </param>
	/// <param name="KnockBackPower"> �m�b�N�o�b�N�� </param>
	/// <param name="Abn"> �^�����Ԉُ� </param>
	/// <param name="AbnTurn"> ��Ԉُ�̃^�[�� </param>
	m_Attack_Datas.emplace_back("�U�� - 1", 5, 1, 0, Abnormality::None, 0);
	m_Attack_Datas.emplace_back("�U�� - 2", 10, 3, 0, Abnormality::None, 0);

	m_Attack_Datas[0].SetAttackRange(1, false);
	m_Attack_Datas[1].SetAttackRange(1, false);

	m_StartUp.m_MoveCost = 4;

	m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn8.png");
}

NoviceBossEnemy::~NoviceBossEnemy()
{
	Enemy::Finalize();
}