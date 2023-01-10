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
	AC_Attack,
	AC_Charge	// ���߂�(�X�^�~�i���񕜂��đ��I��)
};

//-------------------------------------------------------------------------------------------------------------
// �G
//-------------------------------------------------------------------------------------------------------------
class Enemy : public Charactor
{
	Player *pPlayer = nullptr;
	AstarResults m_Route = AstarResults(); // ���[�g�ۑ��p


protected:
	// --------�^�[���J�n��
	struct StartUp {
		// �ړ��\�R�X�g
		int m_MoveCost = 1;
	};
	StartUp m_StartUp;
	//-------------
	
private:
	int m_CoolTime = 0;
	// �ړ��\�R�X�g
	int m_MoveCost;
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

		int m_Range = 1;			// �˒�
		bool m_IsLinear = false;	// �U���������I���ǂ���

		/// <param name="Name"> �Z�� </param>
		/// <param name="Damage"> �_���[�W </param>
		/// <param name="Cost"> �g�p�R�X�g </param>
		/// <param name="KnockBackPower"> �m�b�N�o�b�N�� </param>
		/// <param name="Abn"> �^�����Ԉُ� </param>
		/// <param name="AbnTurn"> ��Ԉُ�̃^�[�� </param>
		Enemy_Attack_Data(std::string Name, int Damage, int Cost, int KnockBackPower, Abnormality Abn, int AbnTurn) :
			m_Name(Name), m_Damage(Damage), m_Cost(Cost), m_KnockBackPower(KnockBackPower), m_Abn(Abn), m_AbnTurn(AbnTurn) {};

		void SetAttackRange(int _Range, bool _IsLinear) {
			m_Range = _Range;
			m_IsLinear = _IsLinear;
		}
	};
	std::vector<Enemy_Attack_Data> m_Attack_Datas;
	
	std::vector<AllResult> AttackAct(int Index, XMINT2 Pos);

	/// <summary>
	/// �U���������邩�ǂ����B������Ȃ�U���\�Ƃ���true��Ԃ�
	/// </summary>
	/// <param name="MapPos">���莞�̎����̈ʒu</param>
	/// <param name="_Attack_Data"></param>
	/// <returns></returns>
	bool AttackHitsTheTarget(XMINT2 MapPos, const Enemy_Attack_Data &_Attack_Data);

	// �S�Ă̍s���ɑ΂��ā@�U���\�ȃX�^�~�i���c���Ă��邩�E�U���������邩 ������Ȃ�C���f�b�N�X���A������Ȃ��Ȃ� -1 ��Ԃ�
	int CanAttack_AttackHitsTheTarget(XMINT2 MapPos);



	// �^�[���J�n���s������
	void StartTurnWhitchAct();
	bool m_GenocideFlag = false; // �v���C���[��|����s�����ł���ꍇ��true�ɂȂ�
	// �ړ����Ȃ���U������
	void Act();
	// �^�[�����I������
	int TurnEndTimer = 20;
	bool TurnEndFlag = false;
	void TurnEnd();

	std::string m_Name; // ���O
	std::string m_JobName = ""; // ���O
	bool m_isBoss = false; // �{�X�G�t���O

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

