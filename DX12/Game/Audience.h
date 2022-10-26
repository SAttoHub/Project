#pragma once
#include "Charactor.h"
#include "Map.h"

//-------------------------------------------------------------------------------------------------------------
// �ϋq�̍s��
//-------------------------------------------------------------------------------------------------------------
enum class AUD_ActionType {
	NONE,
	BOUND,
	ADMISSION,
	EXIT
};

//-------------------------------------------------------------------------------------------------------------
//	�ϋq
//-------------------------------------------------------------------------------------------------------------
class Audience : public Charactor {
private:
	Map *pMap = nullptr;

	static float IndexAngle;
	int m_Step = 0;
	float m_Range = 0.0f;
	int m_Index = 0;
	bool m_DeleteFlag = false;

	XMFLOAT3 m_LocalPos = XMFLOAT3();
	

	AUD_ActionType m_ActionType = AUD_ActionType::NONE;
	static int BoundTimer;
	float BoundHeight = 0;

	static int ExitTimerAll;
	static int ExitTimer1;
	static int ExitTimer2;
	static int ExitTimer3;
	static int ExitTimer4;
	static int ExitTimer5;
	float TargetIndex = 0.0f; // �ޏꎞ�̒i�̒[�̃C���f�b�N�X
	XMFLOAT3 ExitPos = XMFLOAT3();
	XMFLOAT3 ExitPosStep0 = XMFLOAT3();
	XMFLOAT3 ExitPosStep1 = XMFLOAT3();
	XMFLOAT3 ExitPosStep2 = XMFLOAT3();
	XMFLOAT3 ExitPosStep3 = XMFLOAT3();
	XMFLOAT3 ExitPosStep4 = XMFLOAT3();
	
	int ActionCount = 0;
	int ActionTimer = 0;
	void Action();
	void ActionBound();
	void ActionExit();
	void ActionAdmission();

public:
	// x �i�ځ@y ����
	static XMFLOAT2 Step1;
	static XMFLOAT2 Step2;
	static XMFLOAT2 Step3;
	static XMFLOAT2 Step4;
	static XMFLOAT2 Step5;

	Audience();
	~Audience();

	void SetMap(Map *Map);

	void Update() override;
	void Draw() override;
	void SetDir() override;

	int GetStep() { return m_Step; }
	int GetIndex() { return m_Index; }
	bool GetDelete() { return m_DeleteFlag; }
	AUD_ActionType GetAction() { return m_ActionType; }

	/// <summary>
	/// �R���V�A���̂ǂ��ɔz�u���邩
	/// </summary>
	/// <param name="Step">���i�ڂ�</param>
	/// <param name="Index"></param>
	void SetPosition(int Step, int Index);

	void ExitTrigger();
	void AdmissionTrigger();
	void DeleteModel();
};