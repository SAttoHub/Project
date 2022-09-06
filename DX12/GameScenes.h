#pragma once
#include "Singleton.h"
#include "StaticCameraPos.h"

#include "PrimitiveFunc.h"
#include "DrawStrings.h"
#include "ColorFuncs.h"
#include "ColorDefines.h"
#include "3DObject.h"
#include "ModelManager.h"
#include "ExtendedRandom.h"

#include "Player.h"
#include "Enemys.h"
#include "Map.h"
#include "Cursor.h"
#include "Cards.h"

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

	int Turn; // 0 : なし　1 : 敵　2 : プレイヤー

	int GenerateEnemyCount;
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

	void ChangeScene(NowScene s);
};

