#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "Collider/GameCollisionManager.h"

enum class CardType {
	NONE,
	DEFAULT_ATTACK,
	DEFAULT_MOVE
};

class Card
{
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	CardType m_Type = CardType::NONE;
	float width = 0;
	float height = 0;
	XMFLOAT2 LeftTop = XMFLOAT2();

	int Graph = 0;

public:
		GameBaseCollider *collider = nullptr;
		virtual ~Card();
		void SetCollider(GameBaseCollider *collider);
		void ColliderUpdate();
		GameBoxCollider *col = nullptr;
public:
	bool HitCursor = false;
	bool Active = false;
	Card(CardType Type);

	CardType GetType() { return m_Type; }

	void Initialize(int index, int MaxIdx);
	void Update(int index, int MaxIdx);
	void Draw(int index, int MaxIdx);
};

