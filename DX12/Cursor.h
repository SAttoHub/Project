#pragma once
#include "DirectXMathExpansion.h"
#include "PrimitiveFunc.h"
#include "TexManager.h"
#include "GameCollisionManager.h"
#include "Singleton.h"

class Cursor : public Singleton<Cursor> {
	friend class Singleton<Cursor>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	XMFLOAT2 m_MousePos;

public:
	GameBaseCollider *collider = nullptr;
	virtual ~Cursor();
	void SetCollider(GameBaseCollider *collider);
	void ColliderUpdate();
	GamePointCollider *col = nullptr;

public:
	Cursor();

	void Initialize();
	void Update();
	void Draw();
};

