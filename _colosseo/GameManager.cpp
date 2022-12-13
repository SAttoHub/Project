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
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightActive(1, true);
	lightGroup->SetPointLightActive(2, true);
	lightGroup->SetPointLightActive(3, true);
	lightGroup->SetPointLightActive(4, true);
	lightGroup->SetPointLightActive(5, true);
	lightGroup->SetPointLightActive(6, true);
	lightGroup->SetPointLightActive(7, true);
	lightGroup->SetCircleShadowActive(0, false);

	lightGroup->SetDirLightDir(0, XMVECTOR{ 0.0f, -1.0f, 0.0f });
	lightGroup->SetDirLightColor(0, XMFLOAT3{ 1.0f,1.0f,1.0f });

	lightGroup->SetPointLightColor(0, XMFLOAT3{ 1.0f,0.47f,0.0f });
	lightGroup->SetPointLightColor(1, XMFLOAT3{ 1.0f,0.47f,0.0f });
	lightGroup->SetPointLightColor(2, XMFLOAT3{ 1.0f,0.47f,0.0f });
	lightGroup->SetPointLightColor(3, XMFLOAT3{ 1.0f,0.47f,0.0f });
	lightGroup->SetPointLightColor(4, XMFLOAT3{ 1.0f,0.47f,0.0f });
	lightGroup->SetPointLightColor(5, XMFLOAT3{ 1.0f,0.47f,0.0f });
	lightGroup->SetPointLightColor(6, XMFLOAT3{ 1.0f,0.47f,0.0f });
	lightGroup->SetPointLightColor(7, XMFLOAT3{ 1.0f,0.47f,0.0f });

	lightGroup->SetPointLightPos(0, XMFLOAT3{ 56.0f - 70.0f, 10.0f, 56.0f + 10.0f });
	lightGroup->SetPointLightPos(1, XMFLOAT3{ 56.0f - 70.0f, 10.0f, 56.0f - 10.0f });
	lightGroup->SetPointLightPos(2, XMFLOAT3{ 56.0f + 70.0f, 10.0f, 56.0f + 10.0f });
	lightGroup->SetPointLightPos(3, XMFLOAT3{ 56.0f + 70.0f, 10.0f, 56.0f - 10.0f });

	lightGroup->SetPointLightPos(4, XMFLOAT3{ 56.0f + 8.0f, 28.45f, 56.0f - 91.0f });
	lightGroup->SetPointLightPos(5, XMFLOAT3{ 56.0f - 8.0f, 28.45f, 56.0f - 91.0f });
	lightGroup->SetPointLightPos(6, XMFLOAT3{ 56.0f + 8.0f, 28.45f, 56.0f + 91.0f });
	lightGroup->SetPointLightPos(7, XMFLOAT3{ 56.0f - 8.0f, 28.45f, 56.0f + 91.0f });

	lightGroup->SetPointLightAtten(0, XMFLOAT3(0.02f, 0.02f, 0.02f));
	lightGroup->SetPointLightAtten(1, XMFLOAT3(0.02f, 0.02f, 0.02f));
	lightGroup->SetPointLightAtten(2, XMFLOAT3(0.02f, 0.02f, 0.02f));
	lightGroup->SetPointLightAtten(3, XMFLOAT3(0.02f, 0.02f, 0.02f));
	lightGroup->SetPointLightAtten(4, XMFLOAT3(0.02f, 0.02f, 0.02f));
	lightGroup->SetPointLightAtten(5, XMFLOAT3(0.02f, 0.02f, 0.02f));
	lightGroup->SetPointLightAtten(6, XMFLOAT3(0.02f, 0.02f, 0.02f));
	lightGroup->SetPointLightAtten(7, XMFLOAT3(0.02f, 0.02f, 0.02f));


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
