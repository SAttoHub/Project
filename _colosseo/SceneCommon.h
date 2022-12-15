#pragma once
#include "Game/Player.h"
#include "Game/Enemys.h"
#include "Game/Map.h"
#include "Game/Cursor.h"
#include "Game/Cards.h"
#include "Game/Audiences.h"
#include "Game/BattleWave.h"
#include "Game/GameUIsManager.h"
#include "Game/GameCamera.h"

// ëSÉVÅ[Éìã§í ïœêî
class SceneCommon
{
public:
	Player	m_player;
	Enemys	m_enemys;
	Map		m_map;
	Cards	m_cards;
	Audiences m_Audiences;
	GameUI m_GameUI;

	void Initialize();

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
	float m_AddHeightValue;
	float m_CameraRange;
	Camera_Dir m_CamDir;
	void MoveCamera();
};

