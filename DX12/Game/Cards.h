#pragma once
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

	Player *pPlayer = nullptr;
	Enemys *pEnemys = nullptr;
	Map *pMap = nullptr;

	int TurnEndButton = 0;
	bool isMyTurn = false;

public:
	//std::vector<Card> m_Cards;
	My_F_List<Card> m_Cards;

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

	void PlayerTurnUpdate();
	void CardPhaseUpdate();
	void CardEffectPhaseUpdate();

	void Reset(); // 仮リセット
};

