#pragma once
#include "..\Singleton.h"

#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\DrawStrings.h"
#include "..\Engine\Common\Colors\ColorFuncs.h"
#include "..\Engine\Objects\3DObject.h"
#include "..\Engine\Objects\ModelManager.h"
#include "..\Engine\CommonMathIncludes.h"

#include "Player.h"
#include "Enemys.h"
#include "Map.h"
#include "Cursor.h"
#include "Cards.h"
#include "Audiences.h"
#include "BattleWave.h"

enum class NowScene {
	Title,
	StageCelect,
	Game,
	Result,
	Tutorial
};
class GameScenes : public Singleton<GameScenes>
{
	friend class Singleton<GameScenes>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	NowScene Scene;

	Player	m_player;
	Enemys	m_enemys;
	Map		m_map;
	Cards	m_cards;
	Audiences m_Audiences;
	std::unique_ptr<BattleWave> m_BattleWave;
	int NowWave; // 仮Wave
	int EndCount; // 仮ゲーム終了カウント

	int Turn; // 0 : なし　1 : 敵　2 : プレイヤー

	int GenerateEnemyCount;

	int TitleTex;

public:
	void Initialize();
	void Update();
	void Draw();
	void DepthDraw();
	void DOFDepthDraw();
	void BackDraw();
	void ShadowDraw();
	void BloomDraw();
	void BloomDepthDraw();

	void Reset(); // 仮リセット

	void ChangeScene(NowScene s);

	enum Camera_Dir {
		C_FRONT,
		C_FRONTLEFT,
		C_LEFT,
		C_BACKLEFT,
		C_BACK,
		C_BACKRINGHT,
		C_RINGHT,
		C_FRONTRINGHT,
		C_MAX
	};
	float m_CameraAngle;
	float m_CameraHeight;
	float m_CameraRange;
	Camera_Dir m_CamDir;
	void MoveCamera();
};

