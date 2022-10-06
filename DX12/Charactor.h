#pragma once
#include "DirectXMathExpansion.h"
#include "PrimitiveFunc.h"
#include "TexManager.h"
#include "3DObject.h"
#include "DrawStrings.h"

class Charactor
{
public:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	XMINT2 m_MapPos;
	int m_HP;

	Object3d *model;
public:
	Charactor();
	~Charactor();

	void SetMapPos(XMINT2 MapPos) { m_MapPos = MapPos; }
	XMINT2 GetMapPos() { return m_MapPos; }
	int GetHP() { return m_HP; }
	void Damage(int value) { m_HP -= value; }
	// ¶‘¶‚µ‚Ä‚¢‚é‚©”»’è
	bool isAlive() { return m_HP > 0 ? true : false; }

	virtual void Update() = 0;
	virtual void Draw() = 0;
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();
};

