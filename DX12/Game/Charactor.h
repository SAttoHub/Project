#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "..\Engine\Objects\3DObject.h"
#include "..\DrawStrings.h"

// キャラクター状態異常
enum class Abnormality {
	None,
	Poison,
	Stun
};

//-------------------------------------------------------------------------------------------------------------
// キャラクター基底class
//-------------------------------------------------------------------------------------------------------------
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
	XMINT2 m_MapPos = XMINT2();
	int m_HP = 1;
	int m_Def = 0;
	Abnormality m_Abn = Abnormality::None;

	Object3d *model = nullptr;

	// 向き関連
	/*------------------------------------------------
						BACK
			BACKRINGHT			BACKLEFT
	RINGHT									LEFT
			FRONTRINGHT			FRONTLEFT
						FRONT
	------------------------------------------------*/
	enum class Chara_Dir {
		FRONT,
		FRONTRINGHT,
		RINGHT,
		BACKRINGHT,
		BACK,
		BACKLEFT,
		LEFT,
		FRONTLEFT
	};
	Chara_Dir m_Dir = Chara_Dir::FRONT;
	virtual void SetDir();

	int Image[8] = {0};

public:
	Charactor();
	~Charactor();

	XMFLOAT3 GetModelPos() { return model->position; }

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

