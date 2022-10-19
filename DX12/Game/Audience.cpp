#include "Audience.h"

float Audience::IndexAngle = 3.0f;
XMFLOAT2 Audience::Step1 = XMFLOAT2(88.9f, 18.45f);
XMFLOAT2 Audience::Step2 = XMFLOAT2(93.9f, 23.45f);
XMFLOAT2 Audience::Step3 = XMFLOAT2(98.9f, 28.45f);
XMFLOAT2 Audience::Step4 = XMFLOAT2(103.9f, 33.45f);
XMFLOAT2 Audience::Step5 = XMFLOAT2(108.9, 38.45f);
int Audience::BoundTimer = 8;
int Audience::ExitTimerAll = 165;
int Audience::ExitTimer1 = 5;
int Audience::ExitTimer2 = 70;
int Audience::ExitTimer3 = 50;
int Audience::ExitTimer4 = 20;
int Audience::ExitTimer5 = 20;

void Audience::Action()
{
	if (ActionCount == 0 && ActionTimer == 0) {
		m_ActionType = AUD_ActionType::BOUND;
		ActionTimer = BoundTimer;
	}
	else if(ActionCount > 0 && ActionTimer == 0) {
		ActionCount--;
	}

	if (ActionTimer > 0) {
		ActionTimer--;
		if (m_ActionType == AUD_ActionType::BOUND) {
			ActionBound();
		}
		else if (m_ActionType == AUD_ActionType::EXIT) {
			ActionExit();
		}
		else if (m_ActionType == AUD_ActionType::ADMISSION) {
			ActionAdmission();
		}

		if (ActionTimer == 0) {
			m_ActionType = AUD_ActionType::NONE;
			ActionCount = GetRand(50, 100);
		}
	}
}

void Audience::ActionBound()
{
	BoundHeight = sinf((180.0f / float(BoundTimer) * float(ActionTimer)) * M_PI_F / 180.0f) * 3.0f;
	model->position.y = m_LocalPos.y + BoundHeight;
}

void Audience::ExitTrigger()
{
	int IndexHoge = 0;
	if (m_Index > 4 && m_Index < 26) {
		TargetIndex = 27;
		IndexHoge = 1;
	}
	else if (m_Index > 34 && m_Index < 56) {
		TargetIndex = 33;
		IndexHoge = -1;
	}
	else if (m_Index > 64 && m_Index < 86) {
		TargetIndex = 87;
		IndexHoge = 1;
	}
	else if (m_Index > 94 && m_Index < 116) {
		TargetIndex = 93;
		IndexHoge = -1;
	}

	ExitPosStep0.x = cosf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.x;
	ExitPosStep0.y = model->position.y;
	ExitPosStep0.z = sinf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.z;

	if (m_Step == 1) {

		ExitPosStep0.x = cosf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.x;
		ExitPosStep0.y = model->position.y;
		ExitPosStep0.z = sinf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.z;
	}

	ExitPosStep1.x = cosf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.x;
	ExitPosStep1.y = model->position.y;
	ExitPosStep1.z = sinf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.z;

	ExitPosStep2.x = cosf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.x;
	ExitPosStep2.y = Step1.y;
	ExitPosStep2.z = sinf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.z;

	ExitPosStep3.x = cosf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.x;
	ExitPosStep3.y = Step1.y;
	ExitPosStep3.z = sinf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.z;

	ExitPosStep4.x = cosf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (103.9f) + pMap->Center.x;
	ExitPosStep4.y = Step1.y - 5.0f;
	ExitPosStep4.z = sinf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (103.9f) + pMap->Center.z;

	m_ActionType = AUD_ActionType::EXIT;
	ActionTimer = ExitTimerAll;
	ActionCount = 0;
}

void Audience::AdmissionTrigger()
{
	int IndexHoge = 0;
	if (m_Index > 4 && m_Index < 26) {
		TargetIndex = 27;
		IndexHoge = 1;
	}
	else if (m_Index > 34 && m_Index < 56) {
		TargetIndex = 33;
		IndexHoge = -1;
	}
	else if (m_Index > 64 && m_Index < 86) {
		TargetIndex = 87;
		IndexHoge = 1;
	}
	else if (m_Index > 94 && m_Index < 116) {
		TargetIndex = 93;
		IndexHoge = -1;
	}

	ExitPosStep0.x = cosf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.x;
	ExitPosStep0.y = model->position.y;
	ExitPosStep0.z = sinf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.z;

	ExitPosStep1.x = cosf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.x;
	ExitPosStep1.y = model->position.y;
	ExitPosStep1.z = sinf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.z;

	if (m_Step == 1) {
		ExitPosStep0.x = cosf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.x;
		ExitPosStep0.y = model->position.y;
		ExitPosStep0.z = sinf(IndexAngle * float(m_Index) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.z;
		ExitPosStep1.x = cosf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.x;
		ExitPosStep1.y = model->position.y;
		ExitPosStep1.z = sinf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.z;
	}

	ExitPosStep2.x = cosf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.x;
	ExitPosStep2.y = Step1.y;
	ExitPosStep2.z = sinf(IndexAngle * float(TargetIndex) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.z;

	ExitPosStep3.x = cosf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.x;
	ExitPosStep3.y = Step1.y;
	ExitPosStep3.z = sinf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (83.9f) + pMap->Center.z;

	ExitPosStep4.x = cosf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (103.9f) + pMap->Center.x;
	ExitPosStep4.y = Step1.y - 5.0f;
	ExitPosStep4.z = sinf(IndexAngle * float(TargetIndex + IndexHoge * 2) * M_PI_F / 180.0f) * (103.9f) + pMap->Center.z;

	m_ActionType = AUD_ActionType::ADMISSION;
	ActionTimer = ExitTimerAll;
	ActionCount = 0;
}

void Audience::ActionExit()
 {
	float Timer = float(ExitTimerAll) - float(ActionTimer);
	int Timer1 = float(ExitTimer1);
	int Timer2 = float(ExitTimer1) + float(ExitTimer2);
	int Timer3 = float(ExitTimer1) + float(ExitTimer2) + float(ExitTimer3);
	int Timer4 = float(ExitTimer1) + float(ExitTimer2) + float(ExitTimer3) + float(ExitTimer4);
	int Timer5 = float(ExitTimer1) + float(ExitTimer2) + float(ExitTimer3) + float(ExitTimer4) + float(ExitTimer5);

	if (float(Timer) < Timer1) {
		model->position.x = Ease::EaseFunc(EaseName::OutCubic, m_LocalPos.x, ExitPosStep0.x, float(Timer), float(Timer1));
		model->position.z = Ease::EaseFunc(EaseName::OutCubic, m_LocalPos.z, ExitPosStep0.z, float(Timer), float(Timer1));
	}
	else if (float(Timer) < Timer2) {
		float IndexF = Ease::EaseFunc(EaseName::OutCubic, m_Index, TargetIndex, float(Timer - Timer1), float(Timer2 - Timer1));
		model->position.x = cosf(IndexAngle * float(IndexF) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.x;
		model->position.z = sinf(IndexAngle * float(IndexF) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.z;

	}
	else if (float(Timer) < Timer3) {
		if (m_Step == 1) {
			ActionTimer = float(ExitTimerAll) - Timer3;
		}
		model->position.x = Ease::EaseFunc(EaseName::Linear, ExitPosStep1.x, ExitPosStep2.x, float(Timer - Timer2), float(Timer3 - Timer2));
		model->position.y = Ease::EaseFunc(EaseName::Linear, ExitPosStep1.y, ExitPosStep2.y, float(Timer - Timer2), float(Timer3 - Timer2));
		model->position.z = Ease::EaseFunc(EaseName::Linear, ExitPosStep1.z, ExitPosStep2.z, float(Timer - Timer2), float(Timer3 - Timer2));
	}
	else if (float(Timer) < Timer4) {
		model->position.x = Ease::EaseFunc(EaseName::Linear, ExitPosStep2.x, ExitPosStep3.x, float(Timer - Timer3), float(Timer4 - Timer3));
		model->position.z = Ease::EaseFunc(EaseName::Linear, ExitPosStep2.z, ExitPosStep3.z, float(Timer - Timer3), float(Timer4 - Timer3));
	}
	else if (float(Timer) < Timer5) {
		model->position.x = Ease::EaseFunc(EaseName::Linear, ExitPosStep3.x, ExitPosStep4.x, float(Timer - Timer4), float(Timer5 - Timer4));
		model->position.y = Ease::EaseFunc(EaseName::Linear, ExitPosStep3.y, ExitPosStep4.y, float(Timer - Timer4), float(Timer5 - Timer4));
		model->position.z = Ease::EaseFunc(EaseName::Linear, ExitPosStep3.z, ExitPosStep4.z, float(Timer - Timer4), float(Timer5 - Timer4));
	}
	else {
		m_DeleteFlag = true;
	}
}

void Audience::ActionAdmission()
{
	float Timer = float(ActionTimer);
	int Timer1 = float(ExitTimer1);
	int Timer2 = float(ExitTimer1) + float(ExitTimer2);
	int Timer3 = float(ExitTimer1) + float(ExitTimer2) + float(ExitTimer3);
	int Timer4 = float(ExitTimer1) + float(ExitTimer2) + float(ExitTimer3) + float(ExitTimer4);
	int Timer5 = float(ExitTimer1) + float(ExitTimer2) + float(ExitTimer3) + float(ExitTimer4) + float(ExitTimer5);

	if (float(Timer) < Timer1) {
		model->position.x = Ease::EaseFunc(EaseName::OutCubic, m_LocalPos.x, ExitPosStep0.x, float(Timer), float(Timer1));
		model->position.z = Ease::EaseFunc(EaseName::OutCubic, m_LocalPos.z, ExitPosStep0.z, float(Timer), float(Timer1));
	}
	else if (float(Timer) < Timer2) {
		float IndexF = Ease::EaseFunc(EaseName::OutCubic, m_Index, TargetIndex, float(Timer - Timer1), float(Timer2 - Timer1));
		model->position.x = cosf(IndexAngle * float(IndexF) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.x;
		model->position.z = sinf(IndexAngle * float(IndexF) * M_PI_F / 180.0f) * (m_Range - 1) + pMap->Center.z;

	}
	else if (float(Timer) < Timer3) {
		if (m_Step == 1) {
			ActionTimer = float(ExitTimerAll) - Timer2;
		}
		model->position.x = Ease::EaseFunc(EaseName::Linear, ExitPosStep1.x, ExitPosStep2.x, float(Timer - Timer2), float(Timer3 - Timer2));
		model->position.y = Ease::EaseFunc(EaseName::Linear, ExitPosStep1.y, ExitPosStep2.y, float(Timer - Timer2), float(Timer3 - Timer2));
		model->position.z = Ease::EaseFunc(EaseName::Linear, ExitPosStep1.z, ExitPosStep2.z, float(Timer - Timer2), float(Timer3 - Timer2));
	}
	else if (float(Timer) < Timer4) {
		model->position.x = Ease::EaseFunc(EaseName::Linear, ExitPosStep2.x, ExitPosStep3.x, float(Timer - Timer3), float(Timer4 - Timer3));
		model->position.z = Ease::EaseFunc(EaseName::Linear, ExitPosStep2.z, ExitPosStep3.z, float(Timer - Timer3), float(Timer4 - Timer3));
	}
	else if (float(Timer) <= Timer5) {
		model->position.x = Ease::EaseFunc(EaseName::Linear, ExitPosStep3.x, ExitPosStep4.x, float(Timer - Timer4), float(Timer5 - Timer4));
		model->position.y = Ease::EaseFunc(EaseName::Linear, ExitPosStep3.y, ExitPosStep4.y, float(Timer - Timer4), float(Timer5 - Timer4));
		model->position.z = Ease::EaseFunc(EaseName::Linear, ExitPosStep3.z, ExitPosStep4.z, float(Timer - Timer4), float(Timer5 - Timer4));
	}
}

Audience::Audience()
{
	m_MapPos = { 0,0 };
	m_HP = 100;

	int modelData = LoadModelOBJ("Charactor", "mob");
	model = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	model->rotation.y = 90.0f;
	model->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f);
	model->material.texNumber = TexManager::GetColor(XMFLOAT4(0, 255, 255, 255));
	model->isBillboard = true;

	Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaAud1.png");
	Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaAud2.png");
	Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaAud3.png");
	Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaAud4.png");
	Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaAud5.png");
	Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaAud6.png");
	Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaAud7.png");
	Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaAud8.png");
	model->alwaysUpdate = true;

	// 雑に初期化
	m_Step = 0;
	m_Range = 0.0f;
	m_Index = 0;
	m_DeleteFlag = false;
	m_LocalPos = XMFLOAT3();
	m_ActionType = AUD_ActionType::NONE;
	BoundHeight = 0;
	TargetIndex = 0; // 退場時の段の端のインデックス
	ExitPos = XMFLOAT3();
	ExitPosStep0 = XMFLOAT3();
	ExitPosStep1 = XMFLOAT3();
	ExitPosStep2 = XMFLOAT3();
	ExitPosStep3 = XMFLOAT3();
	ExitPosStep4 = XMFLOAT3();
	ActionCount = 0;
	ActionTimer = 0;
}

Audience::~Audience()
{
}

void Audience::SetMap(Map *Map)
{
	pMap = Map;
}

void Audience::Update()
{
	/*float Range = m_Step == 1 ? Step1.x : m_Step == 2 ? Step2.x : m_Step == 3 ? Step3.x : m_Step == 4 ? Step4.x : 0.0f;
	model->position.x = hogePos.x * Range + pMap->Center.x;
	model->position.y = m_Step == 1 ? Step1.y : m_Step == 2 ? Step2.y : m_Step == 3 ? Step3.y : m_Step == 4 ? Step4.y : 0.0f;
	model->position.z = hogePos.z * Range + pMap->Center.z;*/
	Action();

	SetDir();
	model->material.texNumber = Image[(int)m_Dir];
}

void Audience::Draw()
{
	Drawobject3d(model);
}

void Audience::SetDir()
{
	// モデル座標XZとカメラ座標XZとマップ中心座標XZ
	XMFLOAT2 modelPosXZ = XMFLOAT2(model->position.x, model->position.z);
	XMFLOAT2 cameraPosXZ = XMFLOAT2(Camera::eye.x, Camera::eye.z);
	XMFLOAT2 mapCenterXZ = XMFLOAT2(pMap->Center.x, pMap->Center.z);

	// モデル座標からターゲット座標(マップ中心)への方向ベクトルを求める
	XMFLOAT2 FrontVec = modelPosXZ - mapCenterXZ;
	// 方向ベクトルを正規化する
	FrontVec = Normalize2D(FrontVec);

	// モデル→カメラベクトルを正規化する
	XMFLOAT2 norm = Normalize2D(modelPosXZ - cameraPosXZ);
	float cross = CalcAngle(FrontVec, norm);
	//　モデルから見てカメラが右側ならcrossに-1をかける
	if (Smath::GetDirection(modelPosXZ, FrontVec, cameraPosXZ)) {
		cross *= -1;
	}
	// 計算用補正
	cross = cross + 180.0f - 22.5f; // 正面がそのまま正面だと22.5度分ずれるのでずらす
	cross /= 45.0f;
	cross += 0.5f; // 22.5度分ずらしている為、マイナスが出てしまうので補正

	if (cross < 0.5f) m_Dir = Chara_Dir::BACK;
	else if (cross < 1.5f) m_Dir = Chara_Dir::BACKRINGHT;
	else if (cross < 2.5f) m_Dir = Chara_Dir::RINGHT;
	else if (cross < 3.5f) m_Dir = Chara_Dir::FRONTRINGHT;
	else if (cross < 4.5f) m_Dir = Chara_Dir::FRONT;
	else if (cross < 5.5f) m_Dir = Chara_Dir::FRONTLEFT;
	else if (cross < 6.5f) m_Dir = Chara_Dir::LEFT;
	else if (cross < 7.5f) m_Dir = Chara_Dir::BACKLEFT;
}

void Audience::SetPosition(int Step, int Index)
{
	float Range = Step == 1 ? Step1.x : Step == 2 ? Step2.x : Step == 3 ? Step3.x : Step == 4 ? Step4.x : Step == 5 ? Step5.x : 0.0f;
	m_Range = Range;
	XMFLOAT3 Pos;
	static float IndexAngle = 3.0f;
	Pos.x = cosf(IndexAngle * float(Index) * M_PI_F / 180.0f) * Range + pMap->Center.x;
	Pos.y = Step == 1 ? Step1.y : Step == 2 ? Step2.y : Step == 3 ? Step3.y : Step == 4 ? Step4.y : Step == 5 ? Step5.y : 0;
	Pos.z = sinf(IndexAngle * float(Index) * M_PI_F / 180.0f) * Range + pMap->Center.z;

	m_Step = Step;
	m_Index = Index;

	model->position = Pos;
	m_LocalPos = Pos;

	ActionCount = GetRand(50, 100);
	ActionTimer = 0;
	BoundHeight = 0;
	m_DeleteFlag = false;
}
