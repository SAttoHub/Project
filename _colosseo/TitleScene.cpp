#include "TitleScene.h"

void TitleScene::Initialize(SceneCommon* _Common)
{
	Common = _Common;
	m_IsNext = false;
	m_NextScene = SCENE_ID::GAME;

	TitleTex = TexManager::LoadTexture("Resource/image/Title.png");
	m_WaitTimer = -1; // -1の時は次シーンに移行しない
}

void TitleScene::Update()
{
	Cursor::Instance()->Update();
	if (Input::isKeyTrigger(DIK_SPACE)) {
		m_IsNext = true;
	}
}

void TitleScene::Draw()
{
	DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), TitleTex);
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(450, 400), 64, XMFLOAT4(1, 1, 1, 1),
		"SPACE : スタート");
	Common->m_map.Draw();
	Cursor::Instance()->Draw();
}

void TitleScene::DepthDraw()
{
	Common->m_map.DepthDraw();
}

void TitleScene::DOFDepthDraw()
{
	Common->m_map.DOFDepthDraw();
}

void TitleScene::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void TitleScene::ShadowDraw()
{
	Common->m_map.ShadowDraw();
}

void TitleScene::BloomDraw()
{
	Common->m_map.BloomDraw();
}

void TitleScene::BloomDepthDraw()
{
	Common->m_map.BloomDepthDraw();
}
