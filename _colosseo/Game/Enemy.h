#pragma once
#include "Charactor.h"
#include "Map.h"
#include "Astar.h"
#include "..\Input.h"

class Enemys;
class Player;

enum ActPattern {
	AC_Wait,
	AC_Move,
	AC_Attack
};

//-------------------------------------------------------------------------------------------------------------
// �G
//-------------------------------------------------------------------------------------------------------------
class Enemy : public Charactor
{
	Player *pPlayer = nullptr;
	AstarResults m_Route = AstarResults(); // ���[�g�ۑ��p

	int m_CoolTime = 0;
	int m_Count = 0;
	ActPattern m_Act = ActPattern::AC_Wait;

public:
	// �U���̃f�[�^�\����
	struct Enemy_Attack_Data {
		std::string m_Name;		// �Z��
		int m_Damage;			// �_���[�W
		int m_Cost;				// �g�p�R�X�g
		int m_KnockBackPower;	// �m�b�N�o�b�N��
		Abnormality m_Abn;		// �^�����Ԉُ�
		int m_AbnTurn;			// ��Ԉُ�̃^�[��

		/// <param name="Name"> �Z�� </param>
		/// <param name="Damage"> �_���[�W </param>
		/// <param name="Cost"> �g�p�R�X�g </param>
		/// <param name="KnockBackPower"> �m�b�N�o�b�N�� </param>
		/// <param name="Abn"> �^�����Ԉُ� </param>
		/// <param name="AbnTurn"> ��Ԉُ�̃^�[�� </param>
		Enemy_Attack_Data(std::string Name, int Damage, int Cost, int KnockBackPower, Abnormality Abn, int AbnTurn) :
			m_Name(Name), m_Damage(Damage), m_Cost(Cost), m_KnockBackPower(KnockBackPower), m_Abn(Abn), m_AbnTurn(AbnTurn) {};
	};
	std::vector<Enemy_Attack_Data> m_Attack_Datas;
	
	std::vector<AllResult> AttackAct(int Index, XMINT2 Pos);

	std::string m_Name; // ���O

public:
	bool m_MyTurn = false;
	bool m_Next = false;
	Enemy();
	~Enemy();

	void Initialize();
	void Finalize();

	void StartTurn();

	void SetMap(Map *Map);
	void SetPlayer(Player *player);

	void FindRouteToPlayer();

	void Update() override;
	void Draw() override;

	void Death();
};

