#include "Shop.h"
#include "Player.h"

void Shop::Heal_Update()
{
	// プレイヤーの体力が満タンの場合は回復できない
	if (m_pPlayer->GetHP() >= m_pPlayer->GetMaxHP()) {
		m_CanHeal = false;
	}

	XMFLOAT2 point = Input::GetMousePos();
	// 未選択でかつ選択された時
	if (m_CanSelect == true && m_CanHeal ==  true &&
		Input::isMouseTrigger(M_LEFT) &&
		point.x >= m_Heal_LT.x && point.x <= m_Heal_LT.x + m_Heal_Size.x &&
		point.y >= m_Heal_LT.y && point.y <= m_Heal_LT.y + m_Heal_Size.y ) {
		// 選択不可状態にする
		m_CanSelect = false;

		// HPを回復する
		//m_pPlayer->
		m_IsSelectPheseEnd = true;
	}
}

void Shop::Card_Get_Update()
{
	XMFLOAT2 point = Input::GetMousePos();
	// 未選択でかつ選択された時
	if (m_CanSelect == true && m_CanCard_Get == true &&
		Input::isMouseTrigger(M_LEFT) &&
		point.x >= m_Card_Get_LT.x && point.x <= m_Card_Get_LT.x + m_Card_Get_Size.x &&
		point.y >= m_Card_Get_LT.y && point.y <= m_Card_Get_LT.y + m_Card_Get_Size.y) {
		// 選択不可状態にする
		m_CanSelect = false;
		// カード獲得を選択したフラグを建てる
		m_IsSelected_Card_Flag = true;
		// 獲得カードの決定(配列なので -1 する)
		int RandNum = GetRand(1, CHOICES_COUNT) - 1;
		// カードの獲得
		m_pCards->GetCard(m_Choices_Card[RandNum], 1);
		// カード獲得エフェクトを出す
		m_CardEffPtr = std::make_unique<CardGetEff>(m_Choices_Card[RandNum]);
	}
}

Shop::Shop()
{
	m_pCards = nullptr;
	m_pPlayer = nullptr;

	float WID_HARF = float(WINDOW_WIDTH) / 2.0f;
	float HEIG_HARF = float(WINDOW_HEIGHT) / 2.0f;

	// 総合
	m_CanSelect = true;
	// 回復
	m_Heal_Tex = TexManager::LoadTexture("Resource/image/HEAL.png");
	m_Heal_LT = XMFLOAT2(WID_HARF / 4.0f, HEIG_HARF);
	m_Heal_Size = XMFLOAT2(WID_HARF / 2.0f, HEIG_HARF / 2.0f);
	m_CanHeal = true;
	// カード獲得
	m_Card_Get_Tex = TexManager::LoadTexture("Resource/image/RAND_CARD.png");
	m_Card_Get_LT = XMFLOAT2(WID_HARF + WID_HARF / 4.0f, HEIG_HARF);
	m_Card_Get_Size = XMFLOAT2(WID_HARF / 2.0f, HEIG_HARF / 2.0f);
	m_CanCard_Get = true;
	m_IsSelected_Card_Flag = false;

	// カード獲得で獲得できるカードの決定
	m_Choices_Card[0] = CardType::DEFAULT_ATTACK;
	m_Choices_Card[1] = CardType::DEFAULT_MOVE;
	m_Choices_Card[2] = CardType::NEXT_EN_JUMP;


	m_CardEffPtr = nullptr;

	m_IsSelectPheseEnd = false;
	m_IsEnd = false;

	BackTex = TexManager::LoadTexture("Resource/image/REST.png");
}

Shop::~Shop()
{
}

void Shop::SetCardsPtr(Cards* _pCards)
{
	m_pCards = _pCards;
}

void Shop::SetPlayerPtr(Player* _pPlayer)
{
	m_pPlayer = _pPlayer;
}

void Shop::Update()
{
	if (m_IsSelectPheseEnd && Input::isMouseTrigger(M_LEFT)) {
		m_IsEnd = true;
	}


	Heal_Update();
	Card_Get_Update();

	if (m_CardEffPtr != nullptr) {
		m_CardEffPtr->Update();

		if (m_CardEffPtr->GetEnd() == true) {
			m_CardEffPtr.reset();
			m_CardEffPtr = nullptr;
			m_IsSelectPheseEnd = true;

			m_IsEnd = true;
		}
	}
}

void Shop::Draw()
{
	// 背景
	DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), TexManager::GetColor(ColorConvert2(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f))));
	DrawGraph(XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1280.0f, 720.0f), BackTex);
	// 回復
	DrawGraph(m_Heal_LT, m_Heal_LT + m_Heal_Size, m_Heal_Tex);
	if (m_CanHeal == false) {
		DrawGraph(m_Heal_LT, m_Heal_LT + m_Heal_Size, TexManager::GetColor(ColorConvert2(XMFLOAT4(0.1f, 0.1f, 0.1f, 0.5f))));
	}
	// カード獲得
	DrawGraph(m_Card_Get_LT, m_Card_Get_LT + m_Card_Get_Size, m_Card_Get_Tex);

	// カード獲得エフェクト
	if (m_CardEffPtr != nullptr) {
		m_CardEffPtr->Draw();
	}

	if (m_IsSelectPheseEnd) {
		// 色
		const XMFLOAT4 WHITE_COL_B = ColorConvert(XMFLOAT4(230.0f, 242.0f, 255.0f, 255.0f));

		DrawStrings::Instance()->DrawFormatString(XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT) - XMFLOAT2(300.0f, 64.0f),
			64.0f, WHITE_COL_B, "LEFT CLECK END");
	}
}
