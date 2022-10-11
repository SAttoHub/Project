#pragma once
#include "DirectXMathExpansion.h"
#include "PrimitiveFunc.h"
#include "TexManager.h"
#include "3DObject.h"
#include "DrawStrings.h"

// 状態異常
enum class Abnormality {
	None,
	Poison,
	Stun
};

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
	int m_Def;
	Abnormality m_Abn;

	Object3d *model;
public:
	Charactor();
	~Charactor();

	void SetMapPos(XMINT2 MapPos) { m_MapPos = MapPos; }
	XMINT2 GetMapPos() { return m_MapPos; }
	int GetHP() { return m_HP; }
	void Damage(int value) { m_HP -= value; }
	// 生存しているか判定
	bool isAlive() { return m_HP > 0 ? true : false; }
	// 防御力
	int GetDef() { return m_Def; }
	// 状態異常
	Abnormality GetAbn() { return m_Abn; }
	void SetAbn(Abnormality Abn) { m_Abn = Abn; }

	virtual void Update() = 0;
	virtual void Draw() = 0;
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();
};

