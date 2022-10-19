#pragma once
#include <vector>
#include "..\Engine\CommonMathIncludes.h"
#include "Map.h"
#include "Charactor.h"

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

struct ActionResult {
	int m_Damage = 0;
	KnockBack m_KnB = KnockBack();
	Abnormality m_Abn = Abnormality();
	int m_AbnTurn = 0;

	ActionResult(){}
};

struct AllResult {
	short int Index = 0; // ìG : 0 ~ 65534 / ÉvÉåÉCÉÑÅ[ : 65535
	ActionResult Result = ActionResult();
};

class Enemys;
class Player;

class ActionManager
{
	using XMINT2 = DirectX::XMINT2;
	Enemys *pEnemys = nullptr;
	Player *pPlayer = nullptr;

public:
	void Init(Enemys *pEn, Player *pPl);
	std::vector<AllResult> Action(XMINT2 Pos, int Damage, KnockBack KnB, Abnormality Abn, int AbnTurn);

};

