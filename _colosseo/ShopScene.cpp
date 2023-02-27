#include "ShopScene.h"

void ShopScene::Initialize(SceneCommon* _Common)
{
	Common = _Common;

	m_IsNext = false;
	m_NextScene = SCENE_ID::STAGE_SELECT;
	m_WaitTimer = -1; // -1ÇÃéûÇÕéüÉVÅ[ÉìÇ…à⁄çsÇµÇ»Ç¢
	m_HOGE_Timer = 0;

	m_Shop = std::make_unique<Shop>();
	m_Shop->SetCardsPtr(&Common->m_cards);
	m_Shop->SetPlayerPtr(&Common->m_player);

	FadeInStart(30);

	Hoge = false;
}

void ShopScene::Update()
{
	Cursor::Instance()->Update();
	if (m_Shop) {
		m_Shop->Update();

		if (Hoge == false) {
			if (m_Shop->GetEnd() && m_isIn == false && m_isOut == false) {
				//m_IsNext = true;
				FadeStart(30);
				Hoge = true;
			}
		}

		if (m_Shop->GetEnd()) {
			m_HOGE_Timer++;
			if (m_HOGE_Timer >= 28) {
				m_Shop.reset();
				m_Shop = nullptr;
			}
		}
	}

	SceneTimeUpdate();
}

void ShopScene::Draw()
{

	if (m_Shop) {
		m_Shop->Draw();
	}

	Cursor::Instance()->Draw();
	FadeDraw();
}

void ShopScene::DepthDraw()
{
}

void ShopScene::DOFDepthDraw()
{
}

void ShopScene::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void ShopScene::ShadowDraw()
{
}

void ShopScene::BloomDraw()
{
}

void ShopScene::BloomDepthDraw()
{
}
