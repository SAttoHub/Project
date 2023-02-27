#pragma once
#include "Cards.h"

//-------------------------------------------------------------------------------------------------------------
// デッキ編集クラス
//-------------------------------------------------------------------------------------------------------------
class DeckEdit {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	// Card管理クラスのポインタ
	Cards* m_pCards;

	// デッキ編集メニューフラグ
	bool m_IsEditDeck;

	// カードテクスチャ
	int CardTex[int(CardType::CARD_TYPE_MAX)];
	// 背景テクスチャ
	int BackTex;

	// ホールド中のカード情報
	CardType m_HoldCardType;
	// ホールド中のカードのインデックス
	int m_HoldCardIndex;
	// ホールドしているカードがどこのカードか
	int m_HoldCardOwner; // 0 ホールド無し　　1 デッキ　　2　　カードBOX

public:
	DeckEdit();
	~DeckEdit();

	void SetCardsPtr(Cards* _pCards);

	void Udpate();
	void Draw();

private:
	// デッキのIndex番目のカードタイプの取得
	CardType GetDeckCardIndcex(int _Index);
	// カードBOXのIndex番目のカードタイプの取得
	CardType GetBoxCardIndcex(int _Index);

	// デッキのカードとの判定
	int CheckHitCusorDeck();
	// カードBOXのカードとの判定
	int CheckHitCusorBOX();

	// デッキカード裏枠描画
	void DrawDeckBack();
	// デッキカード描画
	void DrawDeckCard();

	// カードBOX裏枠描画
	void DrawCardBoxBack();
	// カードBOXカード描画
	void DrawCardBoxCard();
};