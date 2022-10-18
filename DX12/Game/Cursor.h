#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "Collider/GameCollisionManager.h"
#include "..\Singleton.h"

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

