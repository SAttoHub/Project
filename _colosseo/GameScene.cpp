#include "GameScene.h"
#include "Game/ActionEffectsMgr.h"

void GameScene::Initialize(SceneCommon* _Common)
{
	Common = _Common;
	//m_BattleWave = std::make_unique<BattleWave>(&Common->m_enemys, &Common->m_player);
	//m_BattleWave = std::make_unique<BattleWave>();
	m_BattleWave = Common->m_StageSelect.GetWaveData();
	NowWave = 0;
	EndCount = 0;
	Turn = 2;

	ClearTex = TexManager::LoadTexture("Resource/image/StageClear.png");

	GameCamera::Instance()->Positioning(30.0f, 45.0f, 40.0f, GameCamera::Instance()->DEFAULT_FLAME_TIME);
	GameCamera::Instance()->Targeting(Common->m_map.ChangePos(Common->m_player.GetMapPos()), GameCamera::Instance()->DEFAULT_FLAME_TIME);

	m_BattleWave->NowWaveNum = -1;
	m_BattleWave->NextWaveStart();
	//m_BattleWave->StartWave("Test1");
	Common->m_Audiences.SummonAudience(245);

	m_IsNext = false;
	if (Common->m_StageSelect.GetNowStageData().m_StageName == "Boss") {
		m_NextScene = SCENE_ID::TITLE;
	}
	else {
		m_NextScene = SCENE_ID::STAGE_SELECT;
	}
	m_WaitTimer = -1; // -1の時は次シーンに移行しない

	Common->ResetCamera();

	FadeInStart(30);
}

void GameScene::Update()
{
	Cursor::Instance()->Update();
	ActionEffectsMgr::Instance()->Update();

	if (Common->m_player.GetHP() <= 0) {
		EndCount++;
		if (EndCount > 30) {
			/*DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 300), 64, XMFLOAT4(1, 1, 1, 1),
				"PressR");*/
			if (Input::isKeyTrigger(DIK_R) || Input::isMouseTrigger(M_LEFT)) {
				//m_IsNext = true;
				if (!m_isOut) {
					Common->m_player.Reset();
					Common->m_enemys.AllEnemyClear();
					Common->m_cards.Reset();
					Common->m_cards.StartTurn();
					Common->m_Audiences.DeleteAllAudience();
				}
				FadeStart(30);
			}
		}
	}
	else if (Common->m_enemys.GetEnemyCount() == 0) {

		if (!m_BattleWave->NextWaveStart()) {
			EndCount++;
			if (EndCount > 30) {
				/*DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 300), 64, XMFLOAT4(1, 1, 1, 1),
					"R : タイトルへ");*/
				if (Input::isKeyTrigger(DIK_R) || Input::isMouseTrigger(M_LEFT)) {
					//m_IsNext = true;
					if (!m_isOut) {
						Common->m_player.Reset();
						Common->m_enemys.AllEnemyClear();
						Common->m_cards.Reset();
						Common->m_cards.StartTurn();
						Common->m_Audiences.DeleteAllAudience();
					}
					FadeStart(30);
				}
			}
		}

		//NowWave++;
		//if (NowWave == 1) {
		//	m_BattleWave->StartWave("Test2");
		//}
		//else if (NowWave == 2) {
		//	m_BattleWave->StartWave("Test3");
		//}
		//else if (NowWave == 3) {
		//	m_BattleWave->StartWave("Test4");
		//}  
		//else {
		//	EndCount++;
		//	if (EndCount > 30) {
		//		/*DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 300), 64, XMFLOAT4(1, 1, 1, 1),
		//			"R : タイトルへ");*/
		//		if (Input::isKeyTrigger(DIK_R)) {
		//			m_IsNext = true;
		//			Common->m_player.Reset();
		//			Common->m_enemys.AllEnemyClear();
		//			Common->m_cards.Reset();
		//			Common->m_cards.StartTurn();
		//			Common->m_Audiences.DeleteAllAudience();
		//		}
		//	}
		//	NowWave--;
		//}
	}

	/*if (Common->m_player.GetHP() <= 0) {
		DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 300), 64, XMFLOAT4(1, 1, 1, 1),
			"R : タイトルへ");
		if (Input::isKeyTrigger(DIK_R)) {
			m_IsNext = true;
			Common->m_player.Reset();
			Common->m_cards.Reset();
			Common->m_cards.StartTurn();
			Common->m_Audiences.DeleteAllAudience();
		}
	}*/
	if (m_IsNext == false && Common->m_player.GetHP() > 0) {
		Common->m_player.Update();
		Common->m_enemys.Update();
		Common->m_map.Update();
		// ゲーム終了後にカード選択できないように
		//if (EndCount > 1) {
			Common->m_cards.Update();
		//}
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
	}
	Common->MoveCamera();

	SceneTimeUpdate();
}

void GameScene::Draw()
{
	Common->m_player.Draw();
	Common->m_enemys.Draw();
	Common->m_cards.Draw();
	Common->m_Audiences.Draw();
	Common->m_GameUI.Draw();
	ActionEffectsMgr::Instance()->Draw();

	Common->m_map.Draw();
	Cursor::Instance()->Draw();

	int wave = m_BattleWave->NowWaveNum + 1;
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(0, 0), 64, XMFLOAT4(1, 1, 1, 1),
		"Wave %d/%d", wave < int(m_BattleWave->Waves.size()) ? wave : int(m_BattleWave->Waves.size()), int(m_BattleWave->Waves.size()));

	if (EndCount > 30) {
		/*DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 300), 64, XMFLOAT4(1, 1, 1, 1),
			"R : タイトルへ");*/
		DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), ClearTex);
	}
	FadeDraw();
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
