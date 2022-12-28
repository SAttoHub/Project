#pragma once
#include <vector>
#include "..\Engine\CommonMathIncludes.h"

//-------------------------------------------------------------------------------------------------------------
// �L�����N�^�[��Ԉُ�
//-------------------------------------------------------------------------------------------------------------
enum class Abnormality {
	None,
	Poison,
	Stun
};

//-------------------------------------------------------------------------------------------------------------
// �m�b�N�o�b�N�X�e�[�^�X
//-------------------------------------------------------------------------------------------------------------
struct KnockBack {
	int Power;
	bool UP;
	bool DOWN;
	bool LEFT;
	bool RIGHT;
	KnockBack() : Power(0), UP(false), DOWN(false), LEFT(false), RIGHT(false) {};
	KnockBack(int power, bool up, bool down, bool left, bool right) :
		Power(power), UP(up), DOWN(down), LEFT(left), RIGHT(right) {};
	//KnockBack(KnockBack Knb) :  {};
};

//-------------------------------------------------------------------------------------------------------------
// �s�����N��������
//-------------------------------------------------------------------------------------------------------------
struct ActionResult {
	int m_Damage = 0;
	KnockBack m_KnB = KnockBack();
	Abnormality m_Abn = Abnormality();
	int m_AbnTurn = 0;

	ActionResult(){}
};

//-------------------------------------------------------------------------------------------------------------
// ���ʊi�[�p
//-------------------------------------------------------------------------------------------------------------
struct AllResult {
	short int Index = 0; // �G : 0 ~ 65534 / �v���C���[ : 65535
	ActionResult Result = ActionResult();
};

class Enemys;
class Player;
class Map;

//-------------------------------------------------------------------------------------------------------------
// �U���Ǘ�
//-------------------------------------------------------------------------------------------------------------
class ActionManager
{
	using XMINT2 = DirectX::XMINT2;
	static Enemys *pEnemys;
	static Player *pPlayer;
	static Map* pMap;

public:
	enum ACT_UNIT_TYPE {
		AUT_PL,
		AUT_EN
	};

public:
	void Init(Enemys *pEn, Player *pPl, Map* pM);
	static std::vector<AllResult> Action(XMINT2 Pos, int Damage, KnockBack KnB, Abnormality Abn, int AbnTurn, ACT_UNIT_TYPE AUT_Type);

};

