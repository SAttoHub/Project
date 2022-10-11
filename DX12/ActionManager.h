#pragma once
#include <vector>
#include "DirectXMathExpansion.h"
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
	int m_Damage;
	KnockBack m_KnB;
	Abnormality m_Abn;
	int m_AbnTurn;
};

struct AllResult {
	short int Index; // ìG : 0 ~ 65534 / ÉvÉåÉCÉÑÅ[ : 65535
	ActionResult Result;
};

class Enemys;
class Player;

class ActionManager
{
	using XMINT2 = DirectX::XMINT2;
	Enemys *pEnemys;
	Player *pPlayer;

public:
	void Init(Enemys *pEn, Player *pPl);
	std::vector<AllResult> Action(XMINT2 Pos, int Damage, KnockBack KnB, Abnormality Abn, int AbnTurn);

};

