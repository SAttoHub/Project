#pragma once
#include <vector>
#include "Card.h"
#include "..\Engine\Common\Containers\MyFList.h"
#include "Player.h"
#include "Enemys.h"
#include "Map.h"

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

	int TurnEndButton;
	bool isMyTurn;

public:
	//std::vector<Card> m_Cards;
	My_F_List<Card> m_Cards;

	int NowPhase;
	CardType m_UseCardType;
	int m_UseCardIndex;
	XMINT2 m_NowSelectChip;
	std::vector<XMINT2> CanSelectChips;

	Cards();
	~Cards();

	// ターンエンドボタン
	XMFLOAT2 LT;
	//~Cards();

	void Initialize(Player *_Player, Enemys *_Enemys, Map *_Map);
	void Update();
	void Draw();

	bool TurnEnd;
	void StartTurn();

	bool ExistInCanSelectChips(XMINT2 MapPos);
	void AddCanSelectChips(XMINT2 MapPos);

	void PlayerTurnUpdate();
	void CardPhaseUpdate();
	void CardEffectPhaseUpdate();
};

