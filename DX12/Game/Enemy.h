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

class Enemy : public Charactor
{
	Map *pMap = nullptr;
	Player *pPlayer = nullptr;
	AstarResults m_Route; // ÉãÅ[Égï€ë∂óp

	int coolTime;
	int Count;
	ActPattern Act;
public:
	bool m_MyTurn;
	bool m_Next;
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

