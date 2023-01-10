#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "..\DrawStrings.h"

class Enemys;
class Player;

//-------------------------------------------------------------------------------------------------------------
// 戦闘Wave
//-------------------------------------------------------------------------------------------------------------
class BattleWave
{
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:
	Enemys* pEnemys = nullptr;
	Player* pPlayer = nullptr;

public:
	struct WaveEnemy {
		int Type; // 種類  1 : 剣士　　2 : 魔法使い
		XMINT2 MapPos;
		WaveEnemy(int _Type, XMINT2 _MapPos) : Type(_Type), MapPos(_MapPos) {}
	};
	struct WaveData {
		std::string WaveName;
		std::vector<WaveEnemy> EnemysData;
	};

	
public:
	std::vector<WaveData> Waves;

	BattleWave(/*Enemys *en, Player*pl*/);
	~BattleWave();

	void SetPLandENPtr(Enemys* en, Player* pl);

	void StartWave(std::string WaveName);

	int NowWaveNum = 0;
	bool NextWaveStart();
};

