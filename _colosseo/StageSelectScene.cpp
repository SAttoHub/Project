#include "StageSelectScene.h"

void StageSelectScene::Initialize(SceneCommon* _Common)
{
	Common = _Common;

  	m_IsNext = false;
	m_NextScene = SCENE_ID::GAME;
	m_WaitTimer = -1; // -1‚ÌŽž‚ÍŽŸƒV[ƒ“‚ÉˆÚs‚µ‚È‚¢

	Common->m_StageSelect.SelectStart();

	FadeInStart(30);
}

void StageSelectScene::Update()
{
	Cursor::Instance()->Update();
	Common->m_StageSelect.SelectUpdate();
	if (Common->m_StageSelect.m_isSelect) {
		//m_IsNext = true;
		FadeStart(30);
	}
	SceneTimeUpdate();
}

void StageSelectScene::Draw()
{
	Common->m_StageSelect.Draw();
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
