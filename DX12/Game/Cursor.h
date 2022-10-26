#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "Collider/GameCollisionManager.h"
#include "..\Singleton.h"

//-------------------------------------------------------------------------------------------------------------
// マウスカーソル
//-------------------------------------------------------------------------------------------------------------
class Cursor : public Singleton<Cursor> {
	friend class Singleton<Cursor>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	XMFLOAT2 m_MousePos;

public:
	GameBaseCollider *collider = nullptr;
	virtual ~Cursor();
	void SetCollider();
	void ColliderUpdate();

public:
	Cursor();

	void Initialize();
	void Update();
	void Draw();
};

