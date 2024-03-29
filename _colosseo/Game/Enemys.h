#pragma once
#include <vector>
#include "Enemy.h"

#include "NoviceEnemy.h"
#include "MagicEnemy.h"
#include "NoviceBossEnemy.h"
#include "MagicBossEnemy.h"
#include "ScoutEnemy.h"
#include "ScoutBossEnemy.h"

class Player;

enum EnemyType {
	NoviceEn = 1,
	MagicEn,
	NoviceBoss,
	MagicBoss,
	ScoutEn,
	ScoutBoss
};

//-------------------------------------------------------------------------------------------------------------
// �G�Ǘ�class
//-------------------------------------------------------------------------------------------------------------
class Enemys
{
private:
	Map *pMap = nullptr;
	Player *pPlayer = nullptr;

	bool WaitReaction = false;
	bool WaitReaction2 = false;
public:
	Enemys();
	~Enemys();
	std::vector<Enemy> m_Enemy;

	void SetMap(Map *Map);
	void SetPlayer(Player *player);

	void GenerateEnemy(XMINT2 MapPos, int Type);
	Enemy *GetEnemy(XMINT2 MapPos);
	int GetEnemyIndex(XMINT2 MapPos);
	int GetEnemyCount() { return int(m_Enemy.size()); }

	
	void StartTurn();

	bool NextTurn() { 
		if (m_Enemy.size() > 0) {
			return m_Enemy[m_Enemy.size() - 1].m_Next;
		}
		return false;
	}

	void Initialize();
	void Update();
	void EnemyTurnUpdate();
	void Draw();
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();

	void AllEnemyClear();
	void RandamEnemyGenerate(int Count);
};

