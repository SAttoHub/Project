#pragma once
#include "Cards.h"
#include "CardGetEff.h"

class Player;

class Shop {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	// Card管理クラスのポインタ
	Cards* m_pCards;
	Player* m_pPlayer;
	// 背景テクスチャ
	int BackTex;

	// 総合
	bool m_CanSelect;
	// 回復
	int m_Heal_Tex;
	XMFLOAT2 m_Heal_LT;
	XMFLOAT2 m_Heal_Size;
	// 回復可能かどうか
	bool m_CanHeal;
	void Heal_Update();
	// カード獲得
	int m_Card_Get_Tex;
	XMFLOAT2 m_Card_Get_LT;
	XMFLOAT2 m_Card_Get_Size;
	// カード獲得可能かどうか
	bool m_CanCard_Get;
	// 候補の最大数
	static const int CHOICES_COUNT = 3;
	// 候補カード
	CardType m_Choices_Card[CHOICES_COUNT];
	// カード獲得を選択したか
	bool m_IsSelected_Card_Flag;
	// カード取得エフェクト
	std::unique_ptr<CardGetEff> m_CardEffPtr;
	void Card_Get_Update();

	// 選択終了したか
	bool m_IsSelectPheseEnd;
	// 終了か
	bool m_IsEnd;
public:
	Shop();
	~Shop();

	bool GetEnd() { return m_IsEnd; }

	void SetCardsPtr(Cards* _pCards);
	void SetPlayerPtr(Player* _pPlayer);

	void Update();
	void Draw();
};