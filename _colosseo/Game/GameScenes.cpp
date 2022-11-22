#include "GameScenes.h"
#include "GameCamera.h"

void GameScenes::Initialize()
{
	Scene = NowScene::Title;

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

	m_BattleWave = std::make_unique<BattleWave>(&m_enemys, &m_player);
	NowWave = 0;
	EndCount = 0;

	Turn = 2;
	GenerateEnemyCount = 2;

	TitleTex = TexManager::LoadTexture("Resource/image/Title.png");

	GameCamera::Instance()->Positioning(90.0f, 45.0f, 50.0f, 1);
	GameCamera::Instance()->Targeting(m_map.Center, 1);

	m_CamDir = C_FRONTRINGHT;
	m_CameraAngle = 45.0f;
	m_CameraHeight = 30.0f;
	m_CameraRange = 30.0f;

}

void GameScenes::Update()
{
	/*soko->position = XMFLOAT3(0, -10, 0);*/
	Cursor::Instance()->Update();

	// ランダムに敵を配置してリセット
	/*ImGui::SetNextWindowPos(ImVec2(10, 60), 1);
	ImGui::SetNextWindowSize(ImVec2(300, 100), 1);
	ImGui::Begin("Config");
	ImGui::SliderInt("GenerateEnemyCount", &GenerateEnemyCount, 1, 4);
	if (ImGui::Button("Reset")) {
		m_enemys.AllEnemyClear();
		m_enemys.RandamEnemyGenerate(GenerateEnemyCount);
		Turn = 2;
		m_cards.StartTurn();
	}
	ImGui::End();*/

	if (Scene == NowScene::Title) {
		if (Input::isKeyTrigger(DIK_SPACE)) {
			Scene = NowScene::Game;
			GameCamera::Instance()->Positioning(30.0f, 45.0f, 40.0f, GameCamera::Instance()->DEFAULT_FLAME_TIME);
			GameCamera::Instance()->Targeting(m_player.GetWorldPos(), GameCamera::Instance()->DEFAULT_FLAME_TIME);
			m_BattleWave->StartWave("Test1");
			m_Audiences.SummonAudience(245);
		}
	}
	else if (Scene == NowScene::Game) {
		if (m_enemys.GetEnemyCount() == 0) {
			NowWave++;
			if (NowWave == 1) {
				m_BattleWave->StartWave("Test2");
				/*EndCount++;
				if (EndCount > 30) {
					DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 300), 64, XMFLOAT4(1, 1, 1, 1),
						"R : タイトルへ");
					if (Input::isKeyTrigger(DIK_R)) {
						Reset();
					}
				}
				NowWave--;*/
			}
			else if (NowWave == 2) {
				m_BattleWave->StartWave("Test3");
			}
			else if (NowWave == 3) {
				m_BattleWave->StartWave("Test4");
			}
			else {
				EndCount++;
				if (EndCount > 30) {
					DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 300), 64, XMFLOAT4(1, 1, 1, 1),
						"R : タイトルへ");
					if (Input::isKeyTrigger(DIK_R)) {
						Reset();
					}
				}
				NowWave--;
			}
		}
		m_player.Update();
		m_enemys.Update();
		m_map.Update();
		m_cards.Update();
		m_Audiences.Update();

		if (Turn == 2) {
			m_cards.PlayerTurnUpdate();
			if (m_cards.TurnEnd) {
				Turn = 1; 
				m_enemys.StartTurn();
			}
		}
		else if (Turn == 1) {
			m_enemys.EnemyTurnUpdate();
			if (m_enemys.NextTurn()) {
				m_enemys.m_Enemy[m_enemys.m_Enemy.size() - 1].m_Next = false;
				Turn = 2;
				m_cards.StartTurn();
				//GameCamera::Instance()->Positioning(30.0f, 45.0f, 30.0f, GameCamera::Instance()->DEFAULT_FLAME_TIME);
				GameCamera::Instance()->Positioning(m_CameraRange, m_CameraAngle, m_CameraHeight, GameCamera::Instance()->DEFAULT_FLAME_TIME);
				GameCamera::Instance()->Targeting(m_player.GetWorldPos(), GameCamera::Instance()->DEFAULT_FLAME_TIME);
			}
		}
		MoveCamera();
	}

	/*if (1) {
		m_enemys.StartTurn();
	}*/
}

void GameScenes::Draw()
{
	if (Scene == NowScene::Title) {
		DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), TitleTex);
		DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 400), 64, XMFLOAT4(1, 1, 1, 1),
			"SPACE : スタート");
	}
	if (Scene == NowScene::Game) {
		m_player.Draw();
		m_enemys.Draw();
		m_cards.Draw();
		m_Audiences.Draw();
		m_GameUI.Draw();
	}
	m_map.Draw();
	Cursor::Instance()->Draw();

	if (Input::isKey(DIK_SPACE) && Input::isKeyTrigger(DIK_R)) {
		m_enemys.AllEnemyClear();
		m_enemys.RandamEnemyGenerate(GenerateEnemyCount);
		Turn = 2;
		m_cards.StartTurn();
		Scene = NowScene::Title;
	}
}

void GameScenes::DepthDraw()
{
	if (Scene == NowScene::Game) {
		m_player.DepthDraw();
		m_enemys.DepthDraw();
		m_Audiences.DepthDraw();
	}
	m_map.DepthDraw();
}

void GameScenes::DOFDepthDraw()
{
	if (Scene == NowScene::Game) {
		m_player.DOFDepthDraw();
		m_enemys.DOFDepthDraw();
		m_Audiences.DOFDepthDraw();
	}
	m_map.DOFDepthDraw();
}

void GameScenes::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void GameScenes::ShadowDraw()
{
	if (Scene == NowScene::Game) {
		m_player.ShadowDraw();
		m_enemys.ShadowDraw();
		m_Audiences.ShadowDraw();
	}
	m_map.ShadowDraw();
}

void GameScenes::BloomDraw()
{
	/*if (bloomFlag1) {
		Drawobject3d(saku4);
	}
	if (bloomFlag2) {
		Drawobject3d(saku5);
	}*/
	m_map.BloomDraw();
}

void GameScenes::BloomDepthDraw()
{
	/*if (bloomFlag1) {
		DepthDrawobject3d(saku4);
	}
	if (bloomFlag2) {
		Drawobject3d(saku5);
	}*/
	m_map.BloomDepthDraw();
}

void GameScenes::Reset()
{
	NowWave = 0; // 仮Wave
	EndCount = 0;
	m_player.Reset();
	m_cards.Reset();
	m_cards.StartTurn();
	m_enemys.AllEnemyClear();
	Scene = NowScene::Title;
	m_Audiences.DeleteAllAudience();
	Turn = 2;

	GameCamera::Instance()->Positioning(90.0f, 45.0f, 50.0f, GameCamera::Instance()->DEFAULT_FLAME_TIME);
	GameCamera::Instance()->Targeting(m_map.Center, GameCamera::Instance()->DEFAULT_FLAME_TIME);

	m_CamDir = C_FRONTRINGHT;
	m_CameraAngle = 45.0f;
	m_CameraHeight = 30.0f;
	m_CameraRange = 30.0f;
}

void GameScenes::ChangeScene(NowScene s)
{

}

void GameScenes::MoveCamera()
{
	if (!(Input::isKeyTrigger(DIK_D) || Input::isKeyTrigger(DIK_A) || Input::isKeyTrigger(DIK_W) || Input::isKeyTrigger(DIK_S))) {
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
	const float MIN_CAMERA_HEIGHT = 30.0f;
	const float MAX_CAMERA_HEIGHT = 90.0f;
	if (Input::isKeyTrigger(DIK_W)) {
		if (m_CameraRange > MIN_CAMERA_RANGE) m_CameraRange -= 5.0f;
		if (m_CameraHeight > MIN_CAMERA_HEIGHT) m_CameraHeight -= 5.0f;
	}
	else if (Input::isKeyTrigger(DIK_S)) {
		if (m_CameraRange < MAX_CAMERA_RANGE) m_CameraRange += 5.0f;
		if (m_CameraHeight < MAX_CAMERA_HEIGHT) m_CameraHeight += 5.0f;
	}

	GameCamera::Instance()->Positioning(m_CameraRange, m_CameraAngle, m_CameraHeight, GameCamera::Instance()->DEFAULT_FLAME_TIME);
	//GameCamera::Instance()->Targeting(m_player.GetModelPos(), GameCamera::Instance()->DEFAULT_FLAME_TIME);
}
