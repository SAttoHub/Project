#pragma once
#include "..\Engine\CommonMathIncludes.h"

class Enemy;
class Player;
class Cards;

class PlayerParamUI {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	Enemy* pEnemy;
	Player* pPlayer;
	Cards* pCards;
	XMFLOAT2 m_RefPoint;

	int Img = 0;
	int StateImg = 0;
public:
	PlayerParamUI();
	~PlayerParamUI();

	XMFLOAT2 GetRefPoint() { return m_RefPoint; }
	void SetRefPoint(XMFLOAT2 RefPoint) { m_RefPoint = RefPoint; }

	//void SetEnemyPtr(Enemy* enemy);
	void SetPlayerPtr(Player* player);
	void SetCardsPtr(Cards* cards);

	void Update();
	void Draw();
};