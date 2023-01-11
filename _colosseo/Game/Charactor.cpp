#include "Charactor.h"
#include "..\Camera.h"
#include "..\DrawStrings.h"
#include "GameCamera.h"

#include "ActionManager.h"
#include "Map.h"

const float Charactor::DAMAGE_REACTION_OFFSET_RANGE = 4.0f;
const int Charactor::DAMAGE_REACTION_TIME = 10;

void Charactor::SetDirFromCamera()
{
	// オブジェクトの回転角度から正面を求める
	float Angle = Smath::ToRoundAngle(m_pModel->rotation.y);
	XMFLOAT2 FrontVec = Smath::AngleToDirectionVector(Angle);

	// モデル座標XZとカメラ座標XZ
	XMFLOAT2 modelPosXZ = XMFLOAT2(m_pModel->position.x, m_pModel->position.z);
	XMFLOAT2 cameraPosXZ = XMFLOAT2(Camera::eye.x, Camera::eye.z);
	// モデル→カメラベクトルを正規化する
	XMFLOAT2 norm = Normalize2D(modelPosXZ - cameraPosXZ);
	float cross = CalcAngle(FrontVec, norm);
	//　モデルから見てカメラが右側ならcrossに-1をかける
	if (Smath::GetDirection(modelPosXZ, FrontVec, cameraPosXZ)) {
		cross *= -1;
	}
	// 計算用補正
	cross = cross + 180.0f;
	cross /= 45.0f;

	if (cross < 0.5f) m_DirFromCamera = Chara_Dir::BACK;
	else if (cross < 1.5f) m_DirFromCamera = Chara_Dir::BACKRINGHT;
	else if (cross < 2.5f) m_DirFromCamera = Chara_Dir::RINGHT;
	else if (cross < 3.5f) m_DirFromCamera = Chara_Dir::FRONTRINGHT;
	else if (cross < 4.5f) m_DirFromCamera = Chara_Dir::FRONT;
	else if (cross < 5.5f) m_DirFromCamera = Chara_Dir::FRONTLEFT;
	else if (cross < 6.5f) m_DirFromCamera = Chara_Dir::LEFT;
	else if (cross < 7.5f) m_DirFromCamera = Chara_Dir::BACKLEFT;
	else m_DirFromCamera = Chara_Dir::BACK;

	//DrawStrings::Instance()->DrawFormatString(XMFLOAT2(5, 5), 32, XMFLOAT4(1, 1, 1, 1), "cross%f", cross);


}

Charactor::Charactor()
{
	/*m_MapPos = { 0,0 };
	m_HP = 1;

	int modelData = LoadModelOBJ("Charactor", "mob");
	model = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);*/
	m_Def = 0;
}

Charactor::~Charactor()
{
}

XMFLOAT3 Charactor::GetWorldPos()
{
	return m_pMap->ChangePos(m_MapPos);
}

void Charactor::ShadowDraw()
{
	ShadowDepthDrawobject3d(m_pModel);
	ShadowDepthDrawobject3d(m_pModel, false);
}

void Charactor::DepthDraw()
{
	DepthDrawobject3d(m_pModel);
	DepthDrawobject3d(m_pModel, false);
}

void Charactor::DOFDepthDraw()
{
	DOFDepthDrawobject3d(m_pModel);
}

void Charactor::CameraTargetOnMe(bool _isWait, int _WaitTime)
{
	GameCamera::Instance()->Targeting(GetWorldPos(), _WaitTime);
	if (!_isWait) {
		return;
	}
	Wait(_WaitTime);
}

bool Charactor::isWait()
{
	if (m_WaitTimer > 0) {
		return true;
	}
	return false;
}

void Charactor::WaitUpdate()
{
	if (m_WaitTimer > 0) {
		m_WaitTimer--;
	}
}

XMFLOAT3 Charactor::OffsetDirGet() {

	XMFLOAT2 Dir = XMFLOAT2();
	if (m_ReactionDoDir.x < m_ReactionMakeDir.x) {
		Dir.x = -1.0f;
	}
	else if (m_ReactionDoDir.x > m_ReactionMakeDir.x) {
		Dir.x = 1.0f;
	}
	else {
		Dir.x = 0.0f;
	}
	if (m_ReactionDoDir.y < m_ReactionMakeDir.y) {
		Dir.y = -1.0f;
	}
	else if (m_ReactionDoDir.y > m_ReactionMakeDir.y) {
		Dir.y = 1.0f;
	}
	else {
		Dir.y = 0.0f;
	}
	if (Dir.x != 0.0f && Dir.y != 0.0f) {
		Dir.x = Dir.x / 2.0f;
		Dir.y = Dir.y / 2.0f;
	}

	return XMFLOAT3(Dir.x, 0.0f, Dir.y);
}

XMFLOAT3 Charactor::CameraOffsetDirGet()
{
	XMFLOAT2 Offset = XMFLOAT2();
	float NowAngle = GameCamera::Instance()->GetAngle();
	float ResAngle = Smath::ToRoundAngle(NowAngle);
	if (ResAngle < 45.0f) {
		//Offset = XMFLOAT2(1.0f, 0.0f);
		Offset = XMFLOAT2(0.0f, -1.0f);
	}
	else if (ResAngle < 45.0f * 2.0f) {
		//Offset = XMFLOAT2(0.5f, 0.5f);
		Offset = XMFLOAT2(0.5f, -0.5f);
	}
	else if (ResAngle < 45.0f * 3.0f) {
		//Offset = XMFLOAT2(0.0f, 1.0f);
		Offset = XMFLOAT2(1.0f, 0.0f);
	}
	else if (ResAngle < 45.0f * 4.0f) {
		//Offset = XMFLOAT2(-0.5f, 0.5f);
		Offset = XMFLOAT2(0.5f, 0.5f);
	}
	else if (ResAngle < 45.0f * 5.0f) {
		//Offset = XMFLOAT2(-1.0f, 0.0f);
		Offset = XMFLOAT2(0.0f, 1.0f);
	}
	else if (ResAngle < 45.0f * 6.0f) {
		//Offset = XMFLOAT2(-0.5f, -0.5f);
		Offset = XMFLOAT2(-0.5f, 0.5f);
	}
	else if (ResAngle < 45.0f * 7.0f) {
		//Offset = XMFLOAT2(0.0f, -1.0f);
		Offset = XMFLOAT2(-1.0f, 0.0f);
	}
	else if (ResAngle < 45.0f * 8.0f) {
		//Offset = XMFLOAT2(0.5f, -0.5f);
		Offset = XMFLOAT2(-0.5f, -0.5f);
	}
	return XMFLOAT3(Offset.x, 0.0f, Offset.y);
}

void Charactor::DamageReaction(XMINT2 _ReactionMakeDir, XMINT2 _ReactionDoDir)
{
	m_ReactionMakeDir = _ReactionMakeDir;
	m_ReactionDoDir = _ReactionDoDir;
	m_ReactionTimeMax = DAMAGE_REACTION_TIME;
	m_NowReaction = Reactions::Reac_Damage;
	Wait(DAMAGE_REACTION_TIME);
	// 体力が0なら死亡リアクションに変更
	if (m_HP <= 0) {
		m_NowReaction = Reactions::Reac_Death;
	}
}

void Charactor::NoviceAttackReaction(XMINT2 _ReactionMakeDir, XMINT2 _ReactionDoDir)
{
	m_ReactionMakeDir = _ReactionMakeDir;
	m_ReactionDoDir = _ReactionDoDir;
	m_ReactionTimeMax = DAMAGE_REACTION_TIME;
	m_NowReaction = Reactions::Reac_Novice_Attack;
	m_NotUseWaitTimer = -10;
	// 攻撃タイミング(試験的に実数値で)
	m_ReactionTrigger = 15;

	// 演出側でwaitするので無し
	//Wait(DAMAGE_REACTION_TIME);
}

void Charactor::ReactionUpdate() {
	switch (m_NowReaction)
	{
	case Reac_None:
		m_ReactionOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
		break;
	case Reac_Damage:
		DamageReactionUpdate();
		break;
	case Reac_Death:
		DeathReactionUpdate();
		break;
	case Reac_Novice_Attack:
		NoviceAttackReactionUpdate();
		break;
	default:
		//m_ReactionOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
		break;
	}
}

void Charactor::DamageReactionUpdate() {

	XMFLOAT3 Offset = OffsetDirGet();
	int NowTime = m_ReactionTimeMax - m_WaitTimer;
	if (float(NowTime) < float(m_ReactionTimeMax) * (3.0f / 4.0f)) {
		m_ReactionOffset = Ease::EaseFunc(EaseName::OutQuad, 
			XMFLOAT3(0.0f, 0.0f, 0.0f), Offset * DAMAGE_REACTION_OFFSET_RANGE, NowTime, float(m_ReactionTimeMax) * (3.0f / 4.0f));
	}
	else {
		float T = float(m_ReactionTimeMax) * (3.0f / 4.0f);
		float NowT = float(NowTime) - T;
		float MaxT = float(m_ReactionTimeMax) - T;
		m_ReactionOffset = Ease::EaseFunc(EaseName::OutQuad,
			Offset * DAMAGE_REACTION_OFFSET_RANGE, XMFLOAT3(0.0f, 0.0f, 0.0f), NowT, MaxT);
	}
	if (m_WaitTimer == 0) {
		m_NowReaction = Reactions::Reac_None;
		m_ReactionOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}

void Charactor::NoviceAttackReactionUpdate() {
	m_NotUseWaitTimer++;
	if (m_NotUseWaitTimer < 0) {
		return;
	}
	XMFLOAT3 Offset = OffsetDirGet();
	int NowTime = m_NotUseWaitTimer;
	if (float(NowTime) < float(m_ReactionTimeMax) * (3.0f / 4.0f)) {
		m_ReactionOffset = Ease::EaseFunc(EaseName::OutQuad,
			XMFLOAT3(0.0f, 0.0f, 0.0f), Offset * DAMAGE_REACTION_OFFSET_RANGE, NowTime, float(m_ReactionTimeMax) * (3.0f / 4.0f));
		m_ReactionOffset.y = 2.0f * sinf(((float(NowTime) / (float(m_ReactionTimeMax) * (3.0f / 4.0f))) * 180.0f * M_PI_F) / 180.0f);
	}
	else {
		float T = float(m_ReactionTimeMax) * (3.0f / 4.0f);
		float NowT = float(NowTime) - T;
		float MaxT = float(m_ReactionTimeMax) - T;
		m_ReactionOffset = Ease::EaseFunc(EaseName::OutQuad,
			Offset * DAMAGE_REACTION_OFFSET_RANGE, XMFLOAT3(0.0f, 0.0f, 0.0f), NowT, MaxT);
		m_ReactionOffset.y = 0.0f;
	}
	if (m_NotUseWaitTimer == m_ReactionTimeMax) {
		m_NotUseWaitTimer = 0;
		m_NowReaction = Reactions::Reac_None;
		m_ReactionOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}

void Charactor::DeathReactionUpdate() {
	XMFLOAT3 Offset = OffsetDirGet();
	int NowTime = m_ReactionTimeMax - m_WaitTimer;
	if (float(NowTime) < float(m_ReactionTimeMax)) {
		m_ReactionOffset = Ease::EaseFunc(EaseName::OutQuad,
			XMFLOAT3(0.0f, 0.0f, 0.0f), Offset * DAMAGE_REACTION_OFFSET_RANGE, NowTime, float(m_ReactionTimeMax));
	}
	if (m_WaitTimer == 0) {
		m_NowReaction = Reactions::Reac_Death_End;
	}
}