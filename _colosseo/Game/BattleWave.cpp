#include "BattleWave.h"
#include "Enemys.h"
#include "Player.h"

BattleWave::BattleWave(/*Enemys* en, Player* pl */ )
{
	/*pPlayer = pl;
	pEnemys = en;

	WaveData TestWave1;
	TestWave1.WaveName = "Test1";
	TestWave1.EnemysData.emplace_back(WaveEnemy(1, XMINT2(4, 7)));
	TestWave1.EnemysData.emplace_back(WaveEnemy(1, XMINT2(5, 7)));
	TestWave1.EnemysData.emplace_back(WaveEnemy(1, XMINT2(3, 6)));
	TestWave1.EnemysData.emplace_back(WaveEnemy(2, XMINT2(6, 6)));
	WaveData TestWave2;
	TestWave2.WaveName = "Test2";
	TestWave2.EnemysData.emplace_back(WaveEnemy(1, XMINT2(4, 2)));
	TestWave2.EnemysData.emplace_back(WaveEnemy(1, XMINT2(5, 2)));
	TestWave2.EnemysData.emplace_back(WaveEnemy(2, XMINT2(3, 3)));
	TestWave2.EnemysData.emplace_back(WaveEnemy(1, XMINT2(6, 3)));
	WaveData TestWave3;
	TestWave3.WaveName = "Test3";
	TestWave3.EnemysData.emplace_back(WaveEnemy(1, XMINT2(7, 4)));
	TestWave3.EnemysData.emplace_back(WaveEnemy(2, XMINT2(7, 5)));
	TestWave3.EnemysData.emplace_back(WaveEnemy(1, XMINT2(6, 3)));
	TestWave3.EnemysData.emplace_back(WaveEnemy(2, XMINT2(6, 6)));
	WaveData TestWave4;
	TestWave4.WaveName = "Test4";
	TestWave4.EnemysData.emplace_back(WaveEnemy(2, XMINT2(2, 4)));
	TestWave4.EnemysData.emplace_back(WaveEnemy(2, XMINT2(2, 5)));
	TestWave4.EnemysData.emplace_back(WaveEnemy(2, XMINT2(3, 3)));
	TestWave4.EnemysData.emplace_back(WaveEnemy(1, XMINT2(3, 6)));

	Waves.emplace_back(TestWave1);
	Waves.emplace_back(TestWave2);
	Waves.emplace_back(TestWave3);
	Waves.emplace_back(TestWave4);*/
}

BattleWave::~BattleWave()
{
}

void BattleWave::SetPLandENPtr(Enemys* en, Player* pl)
{
	pPlayer = pl;
	pEnemys = en;
}

void BattleWave::StartWave(std::string WaveName)
{
	WaveData* Wave = nullptr;
	for (auto &wave : Waves) {
		if (wave.WaveName != WaveName) {
			continue;
		}
		Wave = &wave;
		break;
	}
	if (Wave == nullptr) {
		return;
	}
	for (auto& enemy : Wave->EnemysData) {
		if (enemy.MapPos == pPlayer->GetMapPos()) {
			continue;
		}
		pEnemys->GenerateEnemy(enemy.MapPos, enemy.Type);
	}
}

bool BattleWave::NextWaveStart()
{
	NowWaveNum++;
	if (NowWaveNum >= Waves.size()) {
		return false;
	}
	WaveData* Wave = &Waves[NowWaveNum];
	for (auto& enemy : Wave->EnemysData) {
		if (enemy.MapPos == pPlayer->GetMapPos()) {
			continue;
		}
		pEnemys->GenerateEnemy(enemy.MapPos, enemy.Type);
	}
	return true;
}
