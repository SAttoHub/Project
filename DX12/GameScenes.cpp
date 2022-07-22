#include "GameScenes.h"

void GameScenes::Initialize()
{
	Scene = NowScene::Title;

	int modelData = LoadModelOBJ("testStage", "hako");
	testStage = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	testStage->scale = XMFLOAT3(10, 10, 10);
	testStage->material.texNumber = TexManager::GetColor(XMFLOAT4(200, 200, 200, 255));


	int modelData2 = LoadModelOBJ("saku", "hako");
	saku = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	saku->scale = XMFLOAT3(1, 1, 1);
	saku->position = XMFLOAT3(5, 0, 5);

	saku2 = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	saku2->scale = XMFLOAT3(1, 1, 1);
	saku2->position = XMFLOAT3(0, 0, 0);

	saku3 = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	saku3->scale = XMFLOAT3(1, 1, 1);
	saku3->position = XMFLOAT3(-5, 0, -5);

	saku4 = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	saku4->scale = XMFLOAT3(1, 1, 1);
	saku4->position = XMFLOAT3(40, 0, -40);

	saku5 = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	saku5->scale = XMFLOAT3(1, 1, 1);
	saku5->position = XMFLOAT3(-40, 20, 40);

	bloomFlag1 = true;
	bloomFlag2 = false;

	int modelData3 = LoadModelOBJ("soko", "floa");
	soko = DirectX3dObject::CreateObject(GetModelData(modelData3), XMFLOAT3(0, 0, 0), Sea_SHADER);
	soko->scale = XMFLOAT3(100.0f, 100.0f, 100.0f);
	soko->material.texNumber = TexManager::GetColor(XMFLOAT4(10, 150, 150, 255));

	int skydomeDataNum = LoadModelOBJ("skydome", "skydome");
	skydome = DirectX3dObject::CreateObject(GetModelData(skydomeDataNum), XMFLOAT3(0, 0, 0), FBXSHADER);
	skydome->scale = XMFLOAT3(3.0f, 3.0f, 3.0f);

}

void GameScenes::Update()
{
	StaticCameraPos::Instance()->position = DirectX::XMFLOAT3(0, 0, 0);
	StaticCameraPos::Instance()->target = DirectX::XMFLOAT3(0.01f, 0.01f, 1);

	soko->position = XMFLOAT3(0, -10, 0);
}

void GameScenes::Draw()
{
	Drawobject3d(testStage);
	Drawobject3d(soko);
	Drawobject3d(saku);
	Drawobject3d(saku2);
	Drawobject3d(saku3);
	Drawobject3d(saku4);
	Drawobject3d(saku5);
	Drawobject3d(skydome);
}

void GameScenes::DepthDraw()
{
	DepthDrawobject3d(soko);
	DepthDrawobject3d(testStage);
	DepthDrawobject3d(saku);
	DepthDrawobject3d(saku2);
	DepthDrawobject3d(saku3);
	DepthDrawobject3d(saku4);
	DepthDrawobject3d(saku5);
}

void GameScenes::DOFDepthDraw()
{
	DOFDepthDrawobject3d(soko);
	DOFDepthDrawobject3d(testStage);
	DOFDepthDrawobject3d(saku);
	DOFDepthDrawobject3d(saku2);
	DOFDepthDrawobject3d(saku3);
	DOFDepthDrawobject3d(saku4);
	DOFDepthDrawobject3d(saku5);
}

void GameScenes::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void GameScenes::ShadowDraw()
{
	ShadowDepthDrawobject3d(soko);
	ShadowDepthDrawobject3d(testStage);
	ShadowDepthDrawobject3d(saku);
	ShadowDepthDrawobject3d(saku2);
	ShadowDepthDrawobject3d(saku3);
	ShadowDepthDrawobject3d(saku4);
	ShadowDepthDrawobject3d(saku5);
}

void GameScenes::BloomDraw()
{
	if (bloomFlag1) {
		Drawobject3d(saku4);
	}
	if (bloomFlag2) {
		Drawobject3d(saku5);
	}
}

void GameScenes::BloomDepthDraw()
{
	if (bloomFlag1) {
		DepthDrawobject3d(saku4);
	}
	if (bloomFlag2) {
		Drawobject3d(saku5);
	}
}

void GameScenes::ChangeScene(NowScene s)
{

}
