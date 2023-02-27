#include "StageSelectScene.h"

void StageSelectScene::Initialize(SceneCommon* _Common)
{
	Common = _Common;

  	m_IsNext = false;
	m_NextScene = SCENE_ID::GAME;
	m_WaitTimer = -1; // -1の時は次シーンに移行しない
	m_FadeTimer_D = 0;

	Common->m_StageSelect.SelectStart();

	FadeInStart(30);
}

void StageSelectScene::Update()
{
	Cursor::Instance()->Update();
	if (!m_DeckEdit) {
		Common->m_StageSelect.SelectUpdate();
	}

	// デッキ編集メニュー(仮)
	if (Input::isKeyTrigger(DIK_T) && m_DeckEdit == nullptr && m_FadeTimer_D == 0 && m_isIn == false && m_isOut == false) {
		m_FadeTimer_D = 51;
		FadeStart(50, false);
		//m_DeckEdit = std::make_unique<DeckEdit>();
		//m_DeckEdit->SetCardsPtr(&Common->m_cards);
	}

	if (m_FadeTimer_D > 0) {
		m_FadeTimer_D--;
		if (m_FadeTimer_D == 0) {
			m_DeckEdit = std::make_unique<DeckEdit>();
			m_DeckEdit->SetCardsPtr(&Common->m_cards);
			FadeInStart(50, false);
		}
	}

	if (m_DeckEdit) {
		m_DeckEdit->Udpate();

		// 解放
		if (Input::isKeyTrigger(DIK_Y) && m_FadeTimer_D == 0 && m_isIn == false && m_isOut == false) {
			m_FadeTimer_D = -51;
			FadeStart(50, false);

			//m_DeckEdit.reset();
			//m_DeckEdit = nullptr;
		}
	}

	if (m_FadeTimer_D < 0) {
		m_FadeTimer_D++;
		if (m_FadeTimer_D == 0) {
			m_DeckEdit.reset();
			m_DeckEdit = nullptr;
			FadeInStart(50, false);
		}
	}

	if (Common->m_StageSelect.m_isSelect && m_isIn == false && m_isOut == false) {
		//m_IsNext = true;
		if (Common->m_StageSelect.m_NextStageType == StageSelect::StageType::Shop) {
			m_NextScene = SCENE_ID::SHOP;
		}
		else {
			m_NextScene = SCENE_ID::GAME;
		}
		FadeStart(30);
	}
	SceneTimeUpdate();
}

void StageSelectScene::Draw()
{
	Common->m_StageSelect.Draw();

	if (m_DeckEdit) {
		m_DeckEdit->Draw();
	}

	Cursor::Instance()->Draw();
	FadeDraw();
}

void StageSelectScene::DepthDraw()
{
}

void StageSelectScene::DOFDepthDraw()
{
}

void StageSelectScene::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void StageSelectScene::ShadowDraw()
{
}

void StageSelectScene::BloomDraw()
{
}

void StageSelectScene::BloomDepthDraw()
{
}
