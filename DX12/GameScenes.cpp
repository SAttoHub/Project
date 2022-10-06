#include "GameScenes.h"

void GameScenes::Initialize()
{
	Scene = NowScene::Title;

	Cursor::Instance()->Initialize();

	m_map.Initialize();
	m_enemys.SetMap(&m_map);
	m_enemys.SetPlayer(&m_player);
	m_enemys.GenerateEnemy(XMINT2(2, 2));
	m_enemys.GenerateEnemy(XMINT2(4, 6));
	m_player.SetMap(&m_map);

	m_cards.Initialize(&m_player, &m_enemys, &m_map);
	m_cards.StartTurn();

	Turn = 2;
	GenerateEnemyCount = 2;

	TitleTex = TexManager::LoadTexture("Resource/image/Title.png");
}

void GameScenes::Update()
{
	StaticCameraPos::Instance()->position = DirectX::XMFLOAT3(0, 0, 0);
	StaticCameraPos::Instance()->target = DirectX::XMFLOAT3(0.01f, 0.01f, 1);

	/*soko->position = XMFLOAT3(0, -10, 0);*/
	Cursor::Instance()->Update();

	// ランダムに敵を配置してリセット
	ImGui::SetNextWindowPos(ImVec2(10, 60), 1);
	ImGui::SetNextWindowSize(ImVec2(300, 100), 1);
	ImGui::Begin("Config");
	ImGui::SliderInt("GenerateEnemyCount", &GenerateEnemyCount, 1, 4);
	if (ImGui::Button("Reset")) {
		m_enemys.AllEnemyClear();
		m_enemys.RandamEnemyGenerate(GenerateEnemyCount);
		Turn = 2;
		m_cards.StartTurn();
	}
	ImGui::End();

	if (Scene == NowScene::Title) {
		if (Input::isKeyTrigger(DIK_SPACE)) {
			Scene = NowScene::Game;
		}
	}

	if (Scene == NowScene::Game) {
		m_player.Update();
		m_enemys.Update();
		m_map.Update();
		m_cards.Update();

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
			}
		}
	}

	/*if (1) {
		m_enemys.StartTurn();
	}*/
}

void GameScenes::Draw()
{
	if (Scene == NowScene::Title) {
		//DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), TitleTex);
	}
	if (Scene == NowScene::Game) {
		m_player.Draw();
		m_enemys.Draw();
		m_cards.Draw();
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
	}
	m_map.DepthDraw();
}

void GameScenes::DOFDepthDraw()
{
	if (Scene == NowScene::Game) {
		m_player.DOFDepthDraw();
		m_enemys.DOFDepthDraw();
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
}

void GameScenes::BloomDepthDraw()
{
	/*if (bloomFlag1) {
		DepthDrawobject3d(saku4);
	}
	if (bloomFlag2) {
		Drawobject3d(saku5);
	}*/
}

void GameScenes::ChangeScene(NowScene s)
{

}
