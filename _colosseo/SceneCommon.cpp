#include "SceneCommon.h"

void SceneCommon::Initialize()
{
	Cursor::Instance()->Initialize();

	m_map.Initialize();
	m_enemys.SetMap(&m_map);
	m_enemys.SetPlayer(&m_player);

	m_player.SetMap(&m_map);

	m_cards.Initialize(&m_player, &m_enemys, &m_map);
	m_cards.StartTurn();

	m_Audiences.Init(&m_map);

	m_GameUI.GetEnemyParamUI()->SetCardsPtr(&m_cards);
	m_GameUI.GetEnemyParamUI()->SetPlayerPtr(&m_player);
	m_GameUI.GetPlayerParamUI()->SetCardsPtr(&m_cards);
	m_GameUI.GetPlayerParamUI()->SetPlayerPtr(&m_player);

	m_StageSelect.Initialize(&m_enemys, &m_player);


	GameCamera::Instance()->Positioning(90.0f, 45.0f, 50.0f, 1);
	GameCamera::Instance()->Targeting(m_map.Center, 1);

	m_CamDir = C_FRONTRINGHT;
	m_CameraAngle = 90.0f;
	m_CameraHeight = 15.0f;
	m_CameraRange = 30.0f;
}

void SceneCommon::MoveCamera()
{
	if (!(Input::isKeyTrigger(DIK_D) || Input::isKeyTrigger(DIK_A) || Input::isKeyTrigger(DIK_W) || Input::isKeyTrigger(DIK_S) || Input::isKeyTrigger(DIK_Q))) {
		return;
	}
	if (Input::isKeyTrigger(DIK_D)) {
		int d_hoge = int(m_CamDir) + 1;
		if (d_hoge >= int(C_MAX)) d_hoge = int(C_FRONT);
		m_CamDir = Camera_Dir(d_hoge);
		m_CameraAngle += 45.0f;
	}
	else if (Input::isKeyTrigger(DIK_A)) {
		int d_hoge = int(m_CamDir) - 1;
		if (d_hoge < int(C_FRONT)) d_hoge = int(C_FRONTRINGHT);
		m_CamDir = Camera_Dir(d_hoge);
		m_CameraAngle -= 45.0f;
	}


	const float MIN_CAMERA_RANGE = 30.0f;
	const float MAX_CAMERA_RANGE = 90.0f;
	const float MIN_CAMERA_HEIGHT = 15.0f;
	const float MAX_CAMERA_HEIGHT = 75.0f;
	if (Input::isKeyTrigger(DIK_W)) {
		if (m_CameraRange > MIN_CAMERA_RANGE) m_CameraRange -= 5.0f;
		if (m_CameraHeight > MIN_CAMERA_HEIGHT) m_CameraHeight -= 5.0f;
	}
	else if (Input::isKeyTrigger(DIK_S)) {
		if (m_CameraRange < MAX_CAMERA_RANGE) m_CameraRange += 5.0f;
		if (m_CameraHeight < MAX_CAMERA_HEIGHT) m_CameraHeight += 5.0f;
	}

	if (Input::isKeyTrigger(DIK_Q)) {
		m_AddHeightValue == 0.0f ? m_AddHeightValue = 15.0f : m_AddHeightValue = 0.0f;
	}

	GameCamera::Instance()->Positioning(m_CameraRange, m_CameraAngle, m_CameraHeight + m_AddHeightValue, GameCamera::Instance()->DEFAULT_FLAME_TIME);
}

void SceneCommon::ResetCamera()
{
	GameCamera::Instance()->Positioning(m_CameraRange, m_CameraAngle, m_CameraHeight + m_AddHeightValue, GameCamera::Instance()->DEFAULT_FLAME_TIME);
}
