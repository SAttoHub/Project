#include "Enemys.h"
#include "Player.h"
#include "..\Engine\CommonMathIncludes.h"

Enemys::Enemys()
{
}

Enemys::~Enemys()
{
	//delete pMap;
	//delete pPlayer;
}

void Enemys::SetMap(Map *Map)
{
	pMap = Map;
}

void Enemys::SetPlayer(Player *player)
{
	pPlayer = player;
}

void Enemys::GenerateEnemy(XMINT2 MapPos, int Type)
{
	if (MapPos == pPlayer->GetMapPos()) {
		MapPos.x += 1;
	}
	if (MapPos == XMINT2(4, 4) || MapPos == XMINT2(9, 4) || MapPos == XMINT2(4, 9) || MapPos == XMINT2(9, 9)) {
		MapPos.x += 1;
	}

	if (Type == NoviceEn) {
		NoviceEnemy en;
		en.SetMapPos(MapPos);
		en.SetMap(pMap);
		en.SetPlayer(pPlayer);
		m_Enemy.emplace_back(en);
	}
	else if (Type == MagicEn) {
		MagicEnemy en;
		en.SetMapPos(MapPos);
		en.SetMap(pMap);
		en.SetPlayer(pPlayer);
		m_Enemy.emplace_back(en);
	}
	else if (Type == NoviceBoss) {
		NoviceBossEnemy en;
		en.SetMapPos(MapPos);
		en.SetMap(pMap);
		en.SetPlayer(pPlayer);
		m_Enemy.emplace_back(en);
	}
	else if (Type == MagicBoss) {
		MagicBossEnemy en;
		en.SetMapPos(MapPos);
		en.SetMap(pMap);
		en.SetPlayer(pPlayer);
		m_Enemy.emplace_back(en);
	}
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

int Enemys::GetEnemyIndex(XMINT2 MapPos)
{
	if (m_Enemy.size() > 0) {
		for (int i = 0; i < m_Enemy.size(); i++) {
			if (m_Enemy[i].GetMapPos() == MapPos) {
				return i;
			}
		}
	}
	return 0;
}

void Enemys::StartTurn()
{
	//isEnemyTurn = true;
	if (m_Enemy.size() > 0) {
		for (int i = 0; i < m_Enemy.size(); i++) {
			pMap->SetCostTableOnUnitPos(m_Enemy[i].GetMapPos(), 999);
			// いずれかのエネミーがリアクション中である
			if (m_Enemy[i].GetReaction() != Reactions::Reac_None) {
				WaitReaction = true; // リアクションが終わるまで待つ
			}
		}
		if (WaitReaction == false) {
			m_Enemy[0].StartTurn();
		}
	}
}

void Enemys::Initialize()
{
	/*isEnemyTurn = false;
	TurnEnd = false;*/
}

void Enemys::Update()
{
	if (WaitReaction == true) {
		WaitReaction = false;
		for (int i = 0; i < m_Enemy.size(); i++) {
			// いずれかのエネミーがリアクション中である もしくはプレイヤーがリアクション中である
			if (m_Enemy[i].GetReaction() != Reactions::Reac_None || pPlayer->GetReaction() != Reactions::Reac_None) {
				WaitReaction = true; // リアクションが終わるまで待つ
			}
		}
		if (WaitReaction == false) {
			m_Enemy[0].StartTurn();
		}
	}
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
		if (m_Enemy[i].GetHP() <= 0 && m_Enemy[i].GetReaction() == Reactions::Reac_Death_End) {
			m_Enemy[i].Death();
			m_Enemy.erase(m_Enemy.begin() + i);
		}
	}

	// UIに現在選択中のチップの敵情報を送る
	GameUI::GetEnemyParamUI()->SetEnemyPtr(GetEnemy(pMap->NowSelectChip));
}

void Enemys::EnemyTurnUpdate()
{
	for (int i = 0; i < m_Enemy.size(); i++) {
		if (i < m_Enemy.size() - 1) {
			if (m_Enemy[i].m_Next) {
				m_Enemy[i].m_Next = false;
				WaitReaction2 = false;
				for (int j = 0; j < m_Enemy.size(); j++) {
					if (j != i + 1) {
						pMap->SetCostTableOnUnitPos(m_Enemy[j].GetMapPos(), 999);
					}
					// いずれかのエネミーがリアクション中である
					if (m_Enemy[j].GetReaction() != Reactions::Reac_None || pPlayer->GetReaction() != Reactions::Reac_None) {
						WaitReaction2 = true; // リアクションが終わるまで待つ
						m_Enemy[i].m_Next = true;
					}
				}
				if (WaitReaction2 == false) {
					m_Enemy[i + 1].StartTurn();
				}
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
				GenerateEnemy(RandPos, 1);
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
