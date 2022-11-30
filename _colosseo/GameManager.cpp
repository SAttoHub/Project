#include "GameManager.h"
#include "Engine\Objects\Loader\FbxLoader.h"
#include <string>
#include "imguiUse.h"
#include "Engine\Common\Funcs.h"

GameManager::GameManager(DirectX3dObject* OBJECT, TexManager* TEX, Window* Win, PipelineManager* Pipeline, Camera* Cam) {
	this->OBJECT = OBJECT;
	this->TEX = TEX;
	this->Cam = Cam;

	GameCamera::Instance()->Initialize();


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

	m_Scene.Initialize();
}

void GameManager::Init()
{
}

void GameManager::Update() {

	lightGroup->Update();

	GameCamera::Instance()->Update();
	m_Scene.Update();
	m_Scene.BackDraw();
}

void GameManager::Draw() {
	m_Scene.Draw();
}

void GameManager::DepthDraw()
{
	m_Scene.DepthDraw();
}

void GameManager::DOFDepthDraw()
{
	m_Scene.DOFDepthDraw();
}

void GameManager::ShadowDraw()
{
	m_Scene.ShadowDraw();
}

void GameManager::BloomDraw()
{
	m_Scene.BloomDraw();
	while (DirectX3dObject::Counter != 0) {

	}
}

void GameManager::BloomDepthDraw()
{
	m_Scene.BloomDepthDraw();
}
