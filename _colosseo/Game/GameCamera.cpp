#include "GameCamera.h"
#include "..\Input.h"

void GameCamera::Initialize()
{
	Mode = CM_Debug;


	switch (Mode)
	{
	case CM_Debug:
		DebugCamPos = { 0  ,0, 0 };
		Camera::SetCameraPos2(DebugCamPos);
		Camera::Targeting(XMFLOAT3(0, 5, 1));
		Camera::isCameraStop = false;
		break;
	case CM_Game:
		Camera::CursorSetSenter();
		DebugCamPos = { 0 ,0, 0 };
		Camera::Targeting(XMFLOAT3(0, 0, -0));
		Camera::isCameraStop = true;
		Camera::SetCameraPos3(DebugCamPos);
		Camera::isCameraStop = true;
		break;
	default:
		break;
	}

	ShakeValue = XMFLOAT3(0, 0, 0);
	Mode = CM_Game;
	Camera::isCameraStop = true;

	m_PosData.Range = 0.0f;
	m_PosData.Angle = 0.0f;
	m_PosData.Height = 0.0f;
	m_TargetData.Target = XMFLOAT3{ 0,0,0 };
}

void GameCamera::Update()
{
		if (Input::isKeyTrigger(DIK_P)) {
			Mode = CM_Debug;
			Camera::isCameraStop = false;
			DebugCamPos = XMFLOAT3(0, 0, 0);
		}
		if(Input::isKeyTrigger(DIK_E)) {
			Mode = CM_Game;
			Camera::isCameraStop = true;
		}
		if (Input::isKeyTrigger(DIK_R)) {
			Camera::isCameraStop == true ? Camera::isCameraStop = false : Camera::isCameraStop = true;
		}

		switch (Mode)
		{
		case CM_Debug:
			DebugCameraUpdate();
			break;
		case CM_Game:
			GameCameraUpdate();
			break;
		default:
			break;
		}
}

void GameCamera::Finalize()
{
}

	void GameCamera::DebugCameraUpdate()
{
	float MovePower = 1.3f;
	if (Camera::isCameraStop == false) {
		if (Input::keys[DIK_LCONTROL]) {
			MovePower = MovePower / 2;
		}
		if (Input::keys[DIK_W] || Input::IsButton(X_UP) || Input::IsButton(X_Y) || Input::GetRStickTiltDirection(STICK_UP)) {
			DebugCamPos.x += MovePower * sin(Camera::radian.y);
			DebugCamPos.z += MovePower * cos(Camera::radian.y);
		}
		if (Input::keys[DIK_S] || Input::IsButton(X_DOWN) || Input::IsButton(X_A) || Input::GetRStickTiltDirection(STICK_DOWN)) {
			DebugCamPos.x -= MovePower * sin(Camera::radian.y);
			DebugCamPos.z -= MovePower * cos(Camera::radian.y);
		}
		if (Input::keys[DIK_D] || Input::IsButton(X_RIGHT) || Input::IsButton(X_B) || Input::GetRStickTiltDirection(STICK_RIGHT)) {
			DebugCamPos.x += MovePower * sin(Camera::radian.y + M_PI_F / 2.0f);
			DebugCamPos.z += MovePower * cos(Camera::radian.y + M_PI_F / 2.0f);
		}
		if (Input::keys[DIK_A] || Input::IsButton(X_LEFT) || Input::IsButton(X_X) || Input::GetRStickTiltDirection(STICK_LEFT)) {
			DebugCamPos.x -= MovePower * sin(Camera::radian.y + M_PI_F / 2.0f);
			DebugCamPos.z -= MovePower * cos(Camera::radian.y + M_PI_F / 2.0f);
		}

		if (Input::keys[DIK_SPACE]) {
			DebugCamPos.y += MovePower / 2;
		}
		if (Input::keys[DIK_LSHIFT] || Input::keys[DIK_RSHIFT]) {
			DebugCamPos.y -= MovePower / 2;
		}
	}
	Camera::SetCameraPos(m_PosData.Position + DebugCamPos);
	if (Camera::isCameraStop == true) {
		Camera::Targeting(XMFLOAT3(40, 0, 40));
	}

	/*if (Input::isKeyTrigger(DIK_L)) {
		int a = 0;
	}*/
}

void GameCamera::GameCameraUpdate()
{

	//{x=43.1580887 y=18.1999950 z=43.9830246 }
	//{x=16.9675980 y=17.2343636 z=29.3841972 }
	//{x = 38.0959435 y = 49.4000282 z = -39.3253136 }
	//{x = 38.1601181 y = 31.1714096 z = -15.4985771 }

	/*DebugCamPos = XMFLOAT3(38.0959435f, 49.4000282f, -39.3253136f);
	Camera::Targeting(XMFLOAT3(38.1601181f, 31.1714096f, -15.4985771f));*/

	DebugCamPos = XMFLOAT3(-5.585062f, 52.650036f, -20.270103f);
	Camera::Targeting(XMFLOAT3(5.151880f, 32.007656f, -1.333096f));
	Camera::SetCameraPos3(DebugCamPos);

	/*--------- Targeting -----------*/
	m_TargetData.Target = Ease::EaseFunc(EaseName::OutQuad, m_TargetData.AgoTarget, m_TargetData.MarkTarget,
		m_TargetData.NowFlame, m_TargetData.EndFlame);
	if (m_TargetData.NowFlame == m_TargetData.EndFlame) {
		m_TargetData.CanChangeTarget = true;
	}
	else {
		m_TargetData.NowFlame++;
	}

	/*--------- Position -----------*/
	m_PosData.Range = Ease::EaseFunc(EaseName::OutQuad, m_PosData.AgoRange, m_PosData.MarkRange,
		m_PosData.NowFlame[0], m_PosData.EndFlame[0]);
	m_PosData.Angle = Ease::EaseFunc(EaseName::OutQuad, m_PosData.AgoAngle, m_PosData.MarkAngle,
		m_PosData.NowFlame[1], m_PosData.EndFlame[1]);
	m_PosData.Height = Ease::EaseFunc(EaseName::OutQuad, m_PosData.AgoHeight, m_PosData.MarkHeight,
		m_PosData.NowFlame[2], m_PosData.EndFlame[2]);

	if (m_PosData.NowFlame[0] == m_PosData.EndFlame[0]) m_PosData.CanChange[0] = true;
	else m_PosData.NowFlame[0]++;
	if (m_PosData.NowFlame[1] == m_PosData.EndFlame[1]) m_PosData.CanChange[1] = true;
	else m_PosData.NowFlame[1]++;
	if (m_PosData.NowFlame[2] == m_PosData.EndFlame[2]) m_PosData.CanChange[2] = true;
	else m_PosData.NowFlame[2]++;

	XMFLOAT2 CamPosXZ = Smath::AngleToDirectionVector(m_PosData.Angle + 180.0f);
	CamPosXZ = CamPosXZ * m_PosData.Range;
	 
	m_PosData.Position = m_TargetData.Target + XMFLOAT3{ CamPosXZ.x, m_PosData.Height, CamPosXZ.y };

	/*--------- Shake ----------*/
	if (ShakeTimer != 0) {
		ShakeTimer += 5;

		float Power = ShakePower * (float(ShakeTimer) / float(ShakeEnd * 5));
		ShakeValue.x = Power * sinf(float(ShakeTimer) * 3.0f + float(M_PI) / 2.0f);
		ShakeValue.y = 0.0f;
		ShakeValue.z = Power * sinf(float(ShakeTimer) * 4.0f);

		if (ShakeTimer >= ShakeEnd) {
			ShakeTimer = 0;
		}
	}
	else {
		ShakeValue.x = 0.0f;
		ShakeValue.y = 0.0f;
		ShakeValue.z = 0.0f;
	}
	m_PosData.Position += ShakeValue;

	Camera::Targeting(m_TargetData.Target);
	Camera::SetCameraPos3(m_PosData.Position);

	//DebugCamPos = StaticCameraPos::Instance()->position + ShakeValue;

	//Camera::Targeting(StaticCameraPos::Instance()->target + ShakeValue);
	//Camera::SetCameraPos3(DebugCamPos);
}

void GameCamera::Shake(float Power, int Time)
{
	ShakePower = Power;
	ShakeTimer = 1;
	ShakeEnd = Time;
}

void GameCamera::Positioning(float Range, float Angle, float Height, int Flame)
{
	m_PosData.MarkRange = Range;
	m_PosData.AgoRange = m_PosData.Range;

	m_PosData.MarkAngle = Angle;
	m_PosData.AgoAngle = m_PosData.Angle;

	m_PosData.MarkHeight = Height;
	m_PosData.AgoHeight = m_PosData.Height;

	for (int i = 0; i < 3; i++) {
		m_PosData.NowFlame[i] = 0;
		m_PosData.EndFlame[i] = Flame;
		m_PosData.CanChange[i] = false;
	}
}

void GameCamera::ChangeRange(float Range, int Flame)
{
	m_PosData.MarkRange = Range;
	m_PosData.AgoRange = m_PosData.Range;
	m_PosData.NowFlame[0] = 0;
	m_PosData.EndFlame[0] = Flame;
	m_PosData.CanChange[0] = false;
}

void GameCamera::ChangeAngle(float Angle, int Flame)
{
	m_PosData.MarkAngle = Angle;
	m_PosData.AgoAngle = m_PosData.Angle;
	m_PosData.NowFlame[1] = 0;
	m_PosData.EndFlame[1] = Flame;
	m_PosData.CanChange[1] = false;
}

void GameCamera::ChangeHeight(float Height, int Flame)
{
	m_PosData.MarkHeight = Height;
	m_PosData.AgoHeight = m_PosData.Height;
	m_PosData.NowFlame[2] = 0;
	m_PosData.EndFlame[2] = Flame;
	m_PosData.CanChange[2] = false;
}

void GameCamera::Targeting(XMFLOAT3 Target, int Flame)
{
	Target.y = Target.y + 4.0f;
	m_TargetData.AgoTarget = m_TargetData.Target;
	m_TargetData.MarkTarget = Target;
	m_TargetData.NowFlame = 0;
	m_TargetData.EndFlame = Flame;
	m_TargetData.CanChangeTarget = false;
}

void GameCamera::TargetingInstantly(XMFLOAT3 Target)
{
	Target.y = Target.y + 4.0f;
	m_TargetData.AgoTarget = Target;
	m_TargetData.MarkTarget = Target;
	m_TargetData.NowFlame = 0;
	m_TargetData.EndFlame = 0;
	m_TargetData.CanChangeTarget = true;
}
