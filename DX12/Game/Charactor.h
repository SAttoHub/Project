#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "..\Engine\Objects\3DObject.h"
#include "..\DrawStrings.h"

#include "ActionManager.h"
#include "Map.h"

enum Reactions {
	Reac_None,
	Reac_Damage,
	Reac_Death,
	Reac_Death_End
};

//-------------------------------------------------------------------------------------------------------------
// �L�����N�^�[���class
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
	int m_MaxHP = 1;
	int m_Def = 0;
	Abnormality m_Abn = Abnormality::None;

	Object3d *pModel = nullptr;
	Map* pMap = nullptr;

	int m_Stamina = 5;

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
	Chara_Dir m_Dir = Chara_Dir::FRONT;
	virtual void SetDir();

	int Image[8] = {0};

public:
	Charactor();
	~Charactor();

	XMFLOAT3 GetModelPos() { return pModel->position; }

	void SetMapPos(XMINT2 MapPos) { m_MapPos = MapPos; }
	XMINT2 GetMapPos() { return m_MapPos; }
	int GetHP() { return m_HP; }
	int GetMaxHP() { return m_MaxHP; }
	void Damage(int value) { m_HP -= value; }
	// �������Ă��邩����
	bool isAlive() { return m_HP > 0 ? true : false; }
	// �h���
	int GetDef() { return m_Def; }
	// ��Ԉُ�
	Abnormality GetAbn() { return m_Abn; }
	void SetAbn(Abnormality Abn) { m_Abn = Abn; }

	bool UseStamina(int val) {
		if (m_Stamina < val) {
			return false;
		}
		m_Stamina -= val;
		return true;
	}
	int GetStamina() { return m_Stamina; }

	virtual void Update() = 0;
	virtual void Draw() = 0;
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();

	// �҂����Ԋ֘A
protected:
	static const int WAIT_TIMER_VALUE_MOVE = 5; // �ړ����̃J�����҂�����
	static const int WAIT_TIMER_VALUE = 20;
	int m_WaitTimer = 0;
public:
	/// <summary>
	/// �J�����̃^�[�Q�b�g�������ɕύX����
	/// </summary>
	/// <param name="isWait"> �J�����ړ������܂ő҂� </param>
	/// /// <param name="WaitTime"> �҂����� </param>
	void CameraTargetOnMe(bool isWait, int WaitTime = WAIT_TIMER_VALUE);
	void Wait(int value) { m_WaitTimer = value; }
	bool isWaitAndUpdate();

	// ���A�N�V�����֘A
private:
	static const float DAMAGE_REACTION_OFFSET_RANGE;
	static const int DAMAGE_REACTION_TIME;
	Reactions m_NowReaction = Reactions::Reac_None;
	int m_ReactionTimeMax = 0;
	XMINT2 m_ReactionMakeDir = XMINT2();
	XMINT2 m_ReactionDoDir = XMINT2();
	XMFLOAT3 OffsetDirGet();
	XMFLOAT3 CameraOffsetDirGet();
protected:
	XMFLOAT3 m_ReactionOffset = XMFLOAT3();
	void ReactionUpdate();
	void DamageReactionUpdate();
	void DeathReactionUpdate();
public:
	Reactions GetReaction() { return m_NowReaction; }
	void DamageReaction(XMINT2 ReactionMakeDir, XMINT2 ReactionDoDir);
};

