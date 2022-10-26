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
// ìG
//-------------------------------------------------------------------------------------------------------------
class Enemy : public Charactor
{
	Map *pMap = nullptr;
	Player *pPlayer = nullptr;
	AstarResults m_Route = AstarResults(); // ÉãÅ[Égï€ë∂óp

	int coolTime = 0;
	int Count = 0;
	ActPattern Act = ActPattern::AC_Wait;
public:
	bool m_MyTurn = false;
	bool m_Next = false;
	Enemy();
	~Enemy();

	void StartTurn();

	void SetMap(Map *Map);
	void SetPlayer(Player *player);

	void FindRouteToPlayer();

	void Update() override;
	void Draw() override;

	void Death();
};

