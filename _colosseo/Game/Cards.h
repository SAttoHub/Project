#pragma once
#include <algorithm>
#include <vector>
#include "Card.h"
#include "..\Engine\Common\Containers\MyFList.h"
#include "Player.h"
#include "Enemys.h"
#include "Map.h"

//-------------------------------------------------------------------------------------------------------------
// カード管理(デッキ)
//-------------------------------------------------------------------------------------------------------------
class Cards
{
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//My_F_List<Card*> m_Cards;
	//std::vector<Card*> m_Cards;
	// 手札
	std::vector<std::unique_ptr<Card>> m_Cards;

	const short MAX_DECK_COUNT = 40;
	// デッキ
	struct DeckCard {
		// カードタイプ
		CardType Type;
		// 枚数
		short Count;
		DeckCard(CardType _Type, short _Count) : Type(_Type), Count(_Count) {};

		bool operator<(const DeckCard& right) const {
			return Type < right.Type;
		}
	};
	// デッキ
	std::vector<DeckCard> m_Deck_Common;
	// デッキ(バトルシーン用)
	std::vector<CardType> m_Deck_Battle;
	// 所持カード数
	std::vector<DeckCard> m_Card_Have;

private:
	// デッキのリセット
	void ResetDeck();
	// バトルシーンのデッキリセット
	void ResetDeck_Battle();
	// 一枚引く
	void DrawDeck();
	// 所持カードのリセット
	void ResetHaveCard();
public:
	// カードを入手する
	void GetCard(CardType _Type, int _Count);
	// デッキの枚数を取得
	int GetDeckCount();
	// デッキにカードを追加
	void AddDeck(CardType _Type);
private:
	Player *pPlayer = nullptr;
	Enemys *pEnemys = nullptr;
	Map *pMap = nullptr;

	int TurnEndButton = 0;
	int DeckTexture = 0;
	bool isMyTurn = false;

	// 攻撃で与えるダメージの予想値(防御力計算前)
	int PreDamage = 0;

	// 選択一度目はカードを発動しない為の変数
	XMINT2 PreCheckChipOld = XMINT2(-1, -1);
	XMINT2 PreCheckChip = XMINT2(-2, -2);
	bool PreChipExistEnemy = false;

	// このタイマーが0ではない間は入力を受け付けない
	int WaitTimer = 0;
	void Wait(int Value) { WaitTimer = Value; }
public:
	//std::vector<Card> m_Cards;
	
	std::vector<int> m_Deck;

	// 0 : カード選択  1 : カード処理　2 : カード後処理(移動後の向き変更等)
	int NowPhase = 0;
	CardType m_UseCardType = CardType::NONE;
	int m_UseCardIndex = 0;
	XMINT2 m_NowSelectChip = XMINT2();
	std::vector<XMINT2> CanSelectChips;

	Cards();
	~Cards();

	// ターンエンドボタン
	XMFLOAT2 LT = XMFLOAT2();
	//~Cards();

	void Initialize(Player *_Player, Enemys *_Enemys, Map *_Map);
	void Update();
	void Draw();

	bool TurnEnd = false;
	void StartTurn();

	bool ExistInCanSelectChips(XMINT2 MapPos);
	void AddCanSelectChips(XMINT2 MapPos);
	void AddCanSelectChips_Move(XMINT2 MapPos);

	void PlayerTurnUpdate();
	void CardPhaseUpdate();
	void CardEffectPhaseUpdate();
	void CardAfterPhaseUpdate();

	void Reset(); // 仮リセット

public:
	// 攻撃で与えるダメージの予想値(防御力計算前)ゲッタ
	int GetPreDamage() { return PreDamage; }


	// カード画像
	static int CardTexture[int(CardType::CARD_TYPE_MAX)];
};

