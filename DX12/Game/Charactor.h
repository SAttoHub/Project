#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "..\Engine\Objects\3DObject.h"
#include "..\DrawStrings.h"

// ��Ԉُ�
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

	// �����֘A
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
	Chara_Dir m_Dir;
	void SetDir();

	int Image[8];

public:
	Charactor();
	~Charactor();

	XMFLOAT3 GetModelPos() { return model->position; }

	void SetMapPos(XMINT2 MapPos) { m_MapPos = MapPos; }
	XMINT2 GetMapPos() { return m_MapPos; }
	int GetHP() { return m_HP; }
	void Damage(int value) { m_HP -= value; }
	// �������Ă��邩����
	bool isAlive() { return m_HP > 0 ? true : false; }
	// �h���
	int GetDef() { return m_Def; }
	// ��Ԉُ�
	Abnormality GetAbn() { return m_Abn; }
	void SetAbn(Abnormality Abn) { m_Abn = Abn; }

	virtual void Update() = 0;
	virtual void Draw() = 0;
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();
};

