#include "Enemys.h"
#include "Player.h"
#include "ExtendedRandom.h"

void Enemys::SetMap(Map *Map)
{
	pMap = Map;
}

void Enemys::SetPlayer(Player *player)
{
	pPlayer = player;
}

void Enemys::GenerateEnemy(XMINT2 MapPos)
{
	Enemy en;
	en.SetMapPos(MapPos);
	en.SetMap(pMap);
	en.SetPlayer(pPlayer);
	m_Enemy.emplace_back(en);
}

Enemy *Enemys::GetEnemy(XMINT2 MapPos)
{
	for (auto &enemy : m_Enemy) {
		if (enemy.GetMapPos() == MapPos) {
			return &enemy;
		}
	}
    return nullptr;
}

void Enemys::StartTurn()
{
	//isEnemyTurn = true;
	if (m_Enemy.size() > 0) {
		for (int i = 0; i < m_Enemy.size(); i++) {
			pMap->SetCostTableOnUnitPos(m_Enemy[i].GetMapPos(), 999);
		}
		m_Enemy[0].StartTurn();
	}
}

void Enemys::Initialize()
{
	/*isEnemyTurn = false;
	TurnEnd = false;*/
}

void Enemys::Update()
{
	for (auto &enemy : m_Enemy) {
		enemy.Update();
	}

	/*if (isEnemyTurn) {
		for (int i = 0; i < m_Enemy.size(); i++) {
			if (i < m_Enemy.size()) {
				if (m_Enemy[i].m_Next) {
					m_Enemy[i + 1].StartTurn();
				}
			}
			else {
				if (m_Enemy[i].m_Next) {
					isEnemyTurn = false;
					TurnEnd = true;
				}
			}
		}
	}*/

	for (int i = 0; i < m_Enemy.size(); i++) {
		if (m_Enemy[i].GetHP() <= 0) {
			m_Enemy[i].Death();
			m_Enemy.erase(m_Enemy.begin() + i);
		}
	}
}

void Enemys::EnemyTurnUpdate()
{
	for (int i = 0; i < m_Enemy.size(); i++) {
		if (i < m_Enemy.size() - 1) {
			if (m_Enemy[i].m_Next) {
				m_Enemy[i].m_Next = false;
				for (int j = 0; j < m_Enemy.size(); j++) {
					if (j != i + 1) {
						pMap->SetCostTableOnUnitPos(m_Enemy[j].GetMapPos(), 999);
					}
				}
				m_Enemy[i + 1].StartTurn();
				break;
			}
		}
		else {
			if (m_Enemy[i].m_Next) {
				//m_Enemy[i].m_Next = false;
				/*isEnemyTurn = false;
				TurnEnd = true;*/
			}
		}
	}
}

void Enemys::Draw()
{
	for (auto &enemy : m_Enemy) {
		enemy.Draw();
	}
}

void Enemys::ShadowDraw()
{
	for (auto &enemy : m_Enemy) {
		enemy.ShadowDraw();
	}
}

void Enemys::DepthDraw()
{
	for (auto &enemy : m_Enemy) {
		enemy.DepthDraw();
	}
}

void Enemys::DOFDepthDraw()
{
	for (auto &enemy : m_Enemy) {
		enemy.DOFDepthDraw();
	}
}

void Enemys::AllEnemyClear()
{
	for (int i = 0; i < m_Enemy.size(); i++) {
		m_Enemy[i].Death();
	}
	m_Enemy.clear();
}

void Enemys::RandamEnemyGenerate(int Count)
{
	XMINT2 RandPos = XMINT2();
	for (int i = 0; i < Count; i++) {
		RandPos.x = GetRand(0, 9);
		RandPos.y = GetRand(0, 9);
		while (1) {
			if (RandPos != pPlayer->GetMapPos() && GetEnemy(RandPos) == nullptr) {
				GenerateEnemy(RandPos);
				break;
			}
			else {
				RandPos.x = GetRand(0, 9);
				RandPos.y = GetRand(0, 9);
			}
		}
		//if(GetEnemy())
	}
}
