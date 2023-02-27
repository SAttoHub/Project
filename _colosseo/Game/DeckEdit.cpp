#include "DeckEdit.h"

DeckEdit::DeckEdit()
{
	m_pCards = nullptr;
	m_IsEditDeck = true;

	CardTex[int(CardType::NONE)] = TexManager::GetColor(XMFLOAT4(255, 0, 0, 255));
	CardTex[int(CardType::DEFAULT_ATTACK)] = TexManager::LoadTexture("Resource/image/ATK_CARD.png");
	CardTex[int(CardType::DEFAULT_MOVE)] = TexManager::LoadTexture("Resource/image/MOVE_CARD.png");
	CardTex[int(CardType::NEXT_EN_JUMP)] = TexManager::LoadTexture("Resource/image/NEXT_EN_JUMP_CARD.png");

	BackTex = TexManager::LoadTexture("Resource/image/DeckEdit.png");
}

DeckEdit::~DeckEdit()
{
}

void DeckEdit::SetCardsPtr(Cards* _pCards)
{
	m_pCards = _pCards;
}

void DeckEdit::Udpate()
{
	// デッキからカードを削除
	int HitCard = CheckHitCusorDeck();
	if (Input::isMouseTrigger(M_RIGHT) && HitCard != -1) {
		CardType HitType = GetDeckCardIndcex(HitCard);
		if (HitType != CardType::NONE) {
			for (auto& data : m_pCards->m_Deck_Common) {
				if (data.Type == HitType && data.Count > 0) {
					data.Count--;
				}
			}
		}
	}
	// BOXからカードを追加
	HitCard = CheckHitCusorBOX();
	if (Input::isMouseTrigger(M_RIGHT) && HitCard != -1) {
		CardType HitType = GetBoxCardIndcex(HitCard);
		// 当たっているカードがNONEではなくデッキの合計枚数が40未満なら
		if (HitType != CardType::NONE && m_pCards->GetDeckCount() < 40) {
			for (auto& data : m_pCards->m_Card_Have) {
				if (data.Type == HitType) {
					// デッキに入っている枚数
					int ThisType_DeckCount = 0;
					// そのタイプのカードがデッキに何枚入っているか
					for (auto& DECK : m_pCards->m_Deck_Common) {
						if (DECK.Type == HitType) {
							ThisType_DeckCount = int(DECK.Count);
						}
					}
					// カードBOXにある残り枚数
					int CardCount = int(data.Count) - ThisType_DeckCount;
					// BOXにカードがあるなら
					if (CardCount > 0) {
						// デッキにカードを追加
						m_pCards->AddDeck(HitType);
					}
				}
			}
		}
	}

	//デッキのソート
	std::sort(m_pCards->m_Deck_Common.begin(), m_pCards->m_Deck_Common.end());
	std::sort(m_pCards->m_Card_Have.begin(), m_pCards->m_Card_Have.end());
}

void DeckEdit::Draw()
{
	// 背景
	DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), TexManager::GetColor(ColorConvert2(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f))));
	DrawGraph(XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1280.0f, 720.0f), BackTex);
	// デッキ部分
	DrawGraph(XMFLOAT2(0.0f, 130.0f + 75.0f), XMFLOAT2(1280.0f, 315.0f + 75.0f), TexManager::GetColor(XMFLOAT4(0.0f, 85.0f, 145.0f, 150.0f)));

	// デッキカード裏枠
	DrawDeckBack();
	// デッキカード描画
	DrawDeckCard();

	// カードBOX部分
	DrawGraph(XMFLOAT2(320.0f, 375.0f + 75.0f), XMFLOAT2(640.0f, 640.0f + 75.0f), TexManager::GetColor(XMFLOAT4(0.0f, 85.0f, 145.0f, 150.0f)));
	// カードBOXカード裏枠
	DrawCardBoxBack();
	// カードBOXカード描画
	DrawCardBoxCard();
}

CardType DeckEdit::GetDeckCardIndcex(int _Index)
{
	CardType Result = CardType::NONE;
	int Count = -1;
	for (int CTYPE = 0; CTYPE < m_pCards->m_Deck_Common.size(); CTYPE++) {
		for (int CCOUNT = 0; CCOUNT < m_pCards->m_Deck_Common[CTYPE].Count; CCOUNT++) {
			// カウンターを加算
			Count++;
			// カウンターがiと一緒になったら決定
			if (Count == _Index) {
				Result = m_pCards->m_Deck_Common[CTYPE].Type;
				break;
			}
		}
		// カウンターがiと一緒になったら決定
		if (Count == _Index) {
			break;
		}
	}
	return Result;
}

CardType DeckEdit::GetBoxCardIndcex(int _Index)
{
	CardType Result = CardType::NONE;
	int Count = -1;
	for (int CTYPE = 0; CTYPE < m_pCards->m_Card_Have.size(); CTYPE++) {
		// カウンターを加算
		Count++;
		if (Count == _Index) {
			Result = m_pCards->m_Card_Have[CTYPE].Type;
			break;
		}
	}
	return Result;
}

int DeckEdit::CheckHitCusorDeck()
{
	// 一番左のカードの左上
	XMFLOAT2 LT = XMFLOAT2(65.0f, 140.0f + 75.0f);
	// カードサイズ
	XMFLOAT2 CardSize = XMFLOAT2(70.0f, 220.0f) - XMFLOAT2(15.0f, 140.0f);
	// カード裏枠
	float hoge = 0.0f;
	for (int i = 0; i < 40; i++) {
		int index = i;
		if (i > 19) {
			index -= 20;
			LT.y = 225.0f + 75.0f;
		}
		if (i == 20) {
			hoge = 0.0f;
		}

		// 何番目のカードに当たっているかチェック
		XMFLOAT2 B_LT = LT + XMFLOAT2(CardSize.x * index, 0) + XMFLOAT2(hoge, 0);
		XMFLOAT2 B_RB = LT + XMFLOAT2(CardSize.x * (index + 1), CardSize.y) + XMFLOAT2(hoge, 0);
		XMFLOAT2 point = Input::GetMousePos();
		if (point.x >= B_LT.x && point.x <= B_RB.x &&
			point.y >= B_LT.y && point.y <= B_RB.y)
		{
			return i;
		}

		hoge += 5.0f;
	}
	// 何にもあたっていなかったら -1 を返す
	return -1;
}

int DeckEdit::CheckHitCusorBOX()
{
	// 一番左のカードの左上
	XMFLOAT2 LT = XMFLOAT2(330.0f, 385.0f + 75.0f);
	// カードサイズ
	XMFLOAT2 CardSize = XMFLOAT2(70.0f, 220.0f) - XMFLOAT2(15.0f, 140.0f);
	// カード裏枠
	float hoge = 0.0f;
	for (int i = 0; i < 15; i++) {
		int index = i;
		if (i > 9) {
			index -= 10;
			LT.y = 555.0f + 75.0f;
		}
		else if (i > 4) {
			index -= 5;
			LT.y = 470.0f + 75.0f;
		}
		if (i == 5 || i == 10) {
			hoge = 0.0f;
		}

		// 何番目のカードに当たっているかチェック
		XMFLOAT2 B_LT = LT + XMFLOAT2(CardSize.x * index, 0) + XMFLOAT2(hoge, 0);
		XMFLOAT2 B_RB = LT + XMFLOAT2(CardSize.x * (index + 1), CardSize.y) + XMFLOAT2(hoge, 0);
		XMFLOAT2 point = Input::GetMousePos();
		if (point.x >= B_LT.x && point.x <= B_RB.x &&
			point.y >= B_LT.y && point.y <= B_RB.y)
		{
			return i;
		}

		hoge += 5.0f;
	}
	// 何にもあたっていなかったら -1 を返す
	return -1;
}

void DeckEdit::DrawDeckBack()
{
	// 一番左のカードの左上
	XMFLOAT2 LT = XMFLOAT2(65.0f, 140.0f + 75.0f);
	// カードサイズ
	XMFLOAT2 CardSize = XMFLOAT2(70.0f, 220.0f) - XMFLOAT2(15.0f, 140.0f);
	// カード裏枠
	float hoge = 0.0f;
	for (int i = 0; i < 40; i++) {
		int index = i;
		if (i > 19) {
			index -= 20;
			LT.y = 225.0f + 75.0f;
		}
		if (i == 20) {
			hoge = 0.0f;
		}
		DrawGraph(LT + XMFLOAT2(CardSize.x * index, 0) + XMFLOAT2(hoge, 0),
			LT + XMFLOAT2(CardSize.x * (index + 1), CardSize.y) + XMFLOAT2(hoge, 0),
			TexManager::GetColor(XMFLOAT4(30.0f, 30.0f, 30.0f, 150.0f)));
		hoge += 5.0f;
	}
}

void DeckEdit::DrawDeckCard()
{
	// 一番左のカードの左上
	XMFLOAT2 LT = XMFLOAT2(65.0f, 140.0f + 75.0f);
	// カードサイズ
	XMFLOAT2 CardSize = XMFLOAT2(70.0f, 220.0f) - XMFLOAT2(15.0f, 140.0f);
	// カード裏枠
	float hoge = 0.0f;
	for (int i = 0; i < 40; i++) {

		// 描画するカードの決定
		CardType DrawType = GetDeckCardIndcex(i);


		if (DrawType == CardType::NONE) {
			continue;
		}


		int index = i;
		if (i > 19) {
			index -= 20;
			LT.y = 225.0f + 75.0f;
		}
		if (i == 20) {
			hoge = 0.0f;
		}
		DrawGraph(LT + XMFLOAT2(CardSize.x * index, 0) + XMFLOAT2(hoge, 0),
			LT + XMFLOAT2(CardSize.x * (index + 1), CardSize.y) + XMFLOAT2(hoge, 0),
			CardTex[int(DrawType)]);
		hoge += 5.0f;
	}
}

void DeckEdit::DrawCardBoxBack()
{
	// 一番左のカードの左上
	XMFLOAT2 LT = XMFLOAT2(330.0f, 385.0f + 75.0f);
	// カードサイズ
	XMFLOAT2 CardSize = XMFLOAT2(70.0f, 220.0f) - XMFLOAT2(15.0f, 140.0f);
	// カード裏枠
	float hoge = 0.0f;
	for (int i = 0; i < 15; i++) {
		int index = i;
		if (i > 9) {
			index -= 10;
			LT.y = 555.0f + 75.0f;
		}
		else if (i > 4) {
			index -= 5;
			LT.y = 470.0f + 75.0f;
		}
		if (i == 5 || i == 10) {
			hoge = 0.0f;
		}
		DrawGraph(LT + XMFLOAT2(CardSize.x * index, 0) + XMFLOAT2(hoge, 0),
			LT + XMFLOAT2(CardSize.x * (index + 1), CardSize.y) + XMFLOAT2(hoge, 0),
			TexManager::GetColor(XMFLOAT4(30.0f, 30.0f, 30.0f, 150.0f)));
		hoge += 5.0f;
	}
}

void DeckEdit::DrawCardBoxCard()
{
	// 一番左のカードの左上
	XMFLOAT2 LT = XMFLOAT2(330.0f, 385.0f + 75.0f);
	// カードサイズ
	XMFLOAT2 CardSize = XMFLOAT2(70.0f, 220.0f) - XMFLOAT2(15.0f, 140.0f);
	// カード裏枠
	float hoge = 0.0f;
	for (int i = 0; i < 15; i++) {
		// 描画するカードの決定
		CardType DrawType = GetBoxCardIndcex(i);
		if (DrawType == CardType::NONE) {
			continue;
		}

		int index = i;
		if (i > 9) {
			index -= 10;
			LT.y = 555.0f + 75.0f;
		}
		else if (i > 4) {
			index -= 5;
			LT.y = 470.0f + 75.0f;
		}
		if (i == 5 || i == 10) {
			hoge = 0.0f;
		}

		XMFLOAT2 B_LT = LT + XMFLOAT2(CardSize.x * index, 0) + XMFLOAT2(hoge, 0);
		XMFLOAT2 B_RB = LT + XMFLOAT2(CardSize.x * (index + 1), CardSize.y) + XMFLOAT2(hoge, 0);
		DrawGraph(B_LT,
			B_RB,
			CardTex[int(DrawType)]);

		// 枚数等の描画
		for (auto& data : m_pCards->m_Card_Have) {
			if (data.Type == DrawType) {
				// デッキに入っている枚数
				int ThisType_DeckCount = 0;
				// そのタイプのカードがデッキに何枚入っているか
				for (auto& DECK : m_pCards->m_Deck_Common) {
					if (DECK.Type == DrawType) {
						ThisType_DeckCount = int(DECK.Count);
					}
				}
				// カードBOXにある残り枚数
				int CardCount = int(data.Count) - ThisType_DeckCount;

				// 色
				const XMFLOAT4 WHITE_COL_B = ColorConvert(XMFLOAT4(230.0f, 242.0f, 255.0f, 255.0f));
				// 残り枚数が1枚以上なら
				if (CardCount > 0) {
					// 枚数描画
					DrawGraph(B_RB - XMFLOAT2(CardSize.x / 2.0f, CardSize.y / 4.0f),
						B_RB,
						TexManager::GetColor(XMFLOAT4(200, 200, 200, 255)));
					DrawStrings::Instance()->DrawFormatString(B_RB - XMFLOAT2(CardSize.x / 2.0f, CardSize.y / 4.0f) + XMFLOAT2(3.0f, 0.0f),
						CardSize.y / 4.0f, WHITE_COL_B, "x%d", CardCount);
				}
				// 無いなら
				else {
					// カードを暗く
					DrawGraph(B_LT,
						B_RB,
						TexManager::GetColor(XMFLOAT4(30.0f, 30.0f, 30.0f, 150.0f)));
					// 枚数描画
					DrawGraph(B_RB - XMFLOAT2(CardSize.x / 2.0f, CardSize.y / 4.0f),
						B_RB,
						TexManager::GetColor(XMFLOAT4(100, 100, 100, 255)));
					DrawStrings::Instance()->DrawFormatString(B_RB - XMFLOAT2(CardSize.x / 2.0f, CardSize.y / 4.0f) + XMFLOAT2(3.0f, 0.0f),
						CardSize.y / 4.0f, WHITE_COL_B, "x%d", CardCount);
				}
			}
		}


		hoge += 5.0f;
	}
}
