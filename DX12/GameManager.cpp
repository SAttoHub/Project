#include "GameManager.h"
#include "FbxLoader.h"
#include "DebugText.h"
#include <string>
#include "imguiCommon.h"
#include "Funcs.h"

GameManager::GameManager(DirectX3dObject *OBJECT, Sprite *SP, TexManager *TEX, Window *Win, PipelineManager *Pipeline, Camera *Cam) {
	this->OBJECT = OBJECT;
	this->SP = SP;
	this->TEX = TEX;
	this->Cam = Cam;
	//てすと
	Par.DirectX3DObjectReset(Win);
	Emit.Manager = &Par;

	//DebugCamPos = { 0 ,0, 0 };
	//DebugCamPos = { 8 * 0 ,180, -20 };
	//Cam->SetCameraPos2(DebugCamPos);
	//Cam->Targeting(XMFLOAT3(4 * CHIP_SIZE, 3, 4 * CHIP_SIZE));

	//DebugCamPos = { 4 * CHIP_SIZE ,228, 30 };
	//DebugCamPos = { 8 * 0 ,180, -20 };
	//Cam->SetCameraPos2(DebugCamPos);
	//Cam->Targeting(XMFLOAT3(4 * CHIP_SIZE, 0, 4 * CHIP_SIZE));
	//Cam->isCameraStop = false;
	GameCamera::Instance()->Initialize();
	/*

	Cam->CursorSetSenter();
	DebugCamPos = { 51.1955719 ,164.400024, -39.4098396 };
	Cam->Targeting(XMFLOAT3(55.1109428, 139.339188, -23.3903408));
	//Cam->Targeting(XMFLOAT3(4 * CHIP_SIZE, 3, 4 * CHIP_SIZE));
	Cam->isCameraStop = true;
	Cam->SetCameraPos(DebugCamPos);
	//Cam->Targeting(XMFLOAT3(4 * CHIP_SIZE, 3, 4 * CHIP_SIZE));
	Cam->isCameraStop = true;
	//Cam->CursorSetSenter();
	
	*/

	//51.1955719
	//164.400024
	//-39.4098396

	//55.1109428
	//139.339188
	//-23.3903408
	
	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	DirectX3dObject::SetLightGroup(lightGroup);
	lightGroup->SetDirLightActive(0, true);//デフォルトでtrueになっているライトをfalseに
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	lightGroup->SetCircleShadowActive(0, false);

	lightGroup->SetDirLightDir(0, XMVECTOR{ 0, -1, 0 });
	lightGroup->SetDirLightColor(0, XMFLOAT3{ 1,1,1 });

	game.Initialize();
}

void GameManager::Init()
{
}

void GameManager::Update() {
	//imgui
	//imguiCommon *im = imguiCommon::Instance();
	//ImGui_ImplDX12_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();
	//

	//lightGroup->SetCircleShadowCasterPos(0, player::Instance()->GetPos());
	//lightGroup->Se

	lightGroup->Update();
	Emit.update();
	GameCamera::Instance()->Update();
	/*float MovePower = 0.13f * CHIP_SIZE;
	if (Cam->isCameraStop == false) {
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
	Camera::SetCameraPos(DebugCamPos);*/

	game.Update();

	


	game.BackDraw();
}

void GameManager::Draw() {
	Emit.Draw();

	game.Draw();

	Primitive3D::Instance()->Draw();

	DirectXBase::ClearDepthBuffer();

	//imguiCommon::Instance()->CommandExcute(true);
	//デバッグテキスト
	DebugText *debugText = DebugText::GetInstance();
	//debugText->Print("Score : " + to_string(Score::GetScore()), 0, 0, 2);
	DirectXBase::ClearDepthBuffer();
}

void GameManager::DepthDraw()
{
	game.DepthDraw();
}
