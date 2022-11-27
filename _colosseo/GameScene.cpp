#include "GameScene.h"

void GameScene::Initialize(SceneCommon* _Common)
{
	Common = _Common;
	m_BattleWave = std::make_unique<BattleWave>(&Common->m_enemys, &Common->m_player);
	NowWave = 0;
	EndCount = 0;
	Turn = 2;

	GameCamera::Instance()->Positioning(30.0f, 45.0f, 40.0f, GameCamera::Instance()->DEFAULT_FLAME_TIME);
	GameCamera::Instance()->Targeting(Common->m_map.ChangePos(Common->m_player.GetMapPos()), GameCamera::Instance()->DEFAULT_FLAME_TIME);
	m_BattleWave->StartWave("Test1");
	Common->m_Audiences.SummonAudience(245);

	m_IsNext = false;
	m_NextScene = SCENE_ID::TITLE;
	m_WaitTimer = -1; // -1の時は次シーンに移行しない
}

void GameScene::Update()
{
	Cursor::Instance()->Update();

	if (Common->m_enemys.GetEnemyCount() == 0) {
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
					m_IsNext = true;
				}
			}
			NowWave--;
		}
	}
	Common->m_player.Update();
	Common->m_enemys.Update();
	Common->m_map.Update();
	Common->m_cards.Update();
	Common->m_Audiences.Update();

	if (Turn == 2) {
		Common->m_cards.PlayerTurnUpdate();
		if (Common->m_cards.TurnEnd) {
			Turn = 1;
			Common->m_enemys.StartTurn();
		}
	}
	else if (Turn == 1) {
		Common->m_enemys.EnemyTurnUpdate();
		if (Common->m_enemys.NextTurn()) {
			Common->m_enemys.m_Enemy[Common->m_enemys.m_Enemy.size() - 1].m_Next = false;
			Turn = 2;
			Common->m_cards.StartTurn();

			GameCamera::Instance()->Positioning(Common->m_CameraRange, Common->m_CameraAngle, Common->m_CameraHeight, GameCamera::Instance()->DEFAULT_FLAME_TIME);
			GameCamera::Instance()->Targeting(Common->m_map.ChangePos(Common->m_player.GetMapPos()), GameCamera::Instance()->DEFAULT_FLAME_TIME);
		}
	}
	Common->MoveCamera();
}

void GameScene::Draw()
{
	Common->m_player.Draw();
	Common->m_enemys.Draw();
	Common->m_cards.Draw();
	Common->m_Audiences.Draw();
	Common->m_GameUI.Draw();

	Common->m_map.Draw();
	Cursor::Instance()->Draw();
}

void GameScene::DepthDraw()
{
	Common->m_player.DepthDraw();
	Common->m_enemys.DepthDraw();
	Common->m_Audiences.DepthDraw();

	Common->m_map.DepthDraw();
}

void GameScene::DOFDepthDraw()
{
	Common->m_player.DOFDepthDraw();
	Common->m_enemys.DOFDepthDraw();
	Common->m_Audiences.DOFDepthDraw();

	Common->m_map.DOFDepthDraw();
}

void GameScene::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void GameScene::ShadowDraw()
{
	Common->m_player.ShadowDraw();
	Common->m_enemys.ShadowDraw();
	Common->m_Audiences.ShadowDraw();

	Common->m_map.ShadowDraw();
}

void GameScene::BloomDraw()
{
	Common->m_map.BloomDraw();
}

void GameScene::BloomDepthDraw()
{
	Common->m_map.BloomDepthDraw();
}
