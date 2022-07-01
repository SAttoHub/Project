#include "GameCamera.h"
#include "Input.h"

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

	//ShakeValue = XMFLOAT3(0, 0, 0);
	//Mode = CM_Game;
	//Camera::isCameraStop = true;
}

void GameCamera::Update()
{
		if (Input::isKeyTrigger(DIK_Q)) {
			Mode = CM_Debug;
			Camera::isCameraStop = false;
		}
		else if(Input::isKeyTrigger(DIK_E)) {
			Mode = CM_Game;
			Camera::isCameraStop = true;
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
	Camera::SetCameraPos(DebugCamPos);
}

void GameCamera::GameCameraUpdate()
{
	/*DebugCamPos = GetTargetPos();
	Camera::Targeting(XMFLOAT3(CHIP_SIZE * 5.5,0, CHIP_SIZE * 7.5 + 10));
	DebugCamPos.x = CHIP_SIZE * 5.5;
	DebugCamPos.y = CHIP_SIZE * 15;
	DebugCamPos.z = CHIP_SIZE * 7.5 - 30;
	Camera::SetCameraPos3(DebugCamPos);*/

	if (ShakeTimer != 0) {
		ShakeTimer += 5;

		float Power = ShakePower * (float(ShakeTimer) / float(ShakeEnd * 5));
		ShakeValue.x = Power * sinf(ShakeTimer * 3 + M_PI / 2.0f);
		ShakeValue.y = 0.0f;
		ShakeValue.z = Power * sinf(ShakeTimer * 4);

		if (ShakeTimer >= ShakeEnd) {
			ShakeTimer = 0;
		}
	}
	else {
		ShakeValue.x = 0.0f;
		ShakeValue.y = 0.0f;
		ShakeValue.z = 0.0f;
	}

	DebugCamPos = StaticCameraPos::Instance()->position + ShakeValue;
	//Camera::Targeting(StaticCameraPos::Instance()->target + ShakeValue);
	//Camera::SetCameraPos3(DebugCamPos);
}

void GameCamera::Shake(float Power, int Time)
{
	ShakePower = Power;
	ShakeTimer = 1;
	ShakeEnd = Time;
}