#pragma once
#include "SceneBase.h"
#include "Game/DeckEdit.h"
#include "Game/Shop.h"

class ShopScene : public SceneBase
{
	// デッキ編集メニュー
	//std::unique_ptr<DeckEdit> m_DeckEdit = nullptr;
	// デッキ編集メニュー表示フェードタイマー
	//int m_FadeTimer_D;


	std::unique_ptr<Shop> m_Shop = nullptr;
	int m_HOGE_Timer;
	bool Hoge;

public:
	void Initialize(SceneCommon* _Common) override;
	void Update() override;
	void Draw() override;
	void DepthDraw() override;
	void DOFDepthDraw() override;
	void BackDraw() override;
	void ShadowDraw() override;
	void BloomDraw() override;
	void BloomDepthDraw() override;
};
