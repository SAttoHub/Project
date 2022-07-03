#include "GameScenes.h"

void GameScenes::Initialize()
{
	Scene = NowScene::Title;
	int modelData = LoadModelOBJ("Hako", "hako");
	model = DirectX3dObject::CreateObject(GetModelData(modelData), XMFLOAT3(0, 0, 0), FBXSHADER);
	model->scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	int modelData2 = LoadModelOBJ("Hako", "flo");
	Sea = DirectX3dObject::CreateObject(GetModelData(modelData2), XMFLOAT3(0, 0, 0), FBXSHADER);
	//Sea->scale = XMFLOAT3(10.0f, 10.0f, 10.0f);

	int modelData3 = LoadModelOBJ("soko", "floa");
	soko = DirectX3dObject::CreateObject(GetModelData(modelData3), XMFLOAT3(0, 0, 0), FBXSHADER);
	//soko->scale = XMFLOAT3(100.0f, 100.0f, 100.0f);

	Sea->position = XMFLOAT3(0, 0, 0);

	Sea->color.w = 0.55f;

	int skydomeDataNum = LoadModelOBJ("skydome", "skydome");
	skydome = DirectX3dObject::CreateObject(GetModelData(skydomeDataNum), XMFLOAT3(0, 0, 0), FBXSHADER);
	skydome->scale = XMFLOAT3(3.0f, 3.0f, 3.0f);

	squere = DirectX3dObject::CreateObject(GetModelData(skydomeDataNum), XMFLOAT3(0, 1, 0), FBXSHADER);
	squere->scale = XMFLOAT3(0.001f, 0.001f, 0.001f);
	squere->material.texNumber = TexManager::GetColor(XMFLOAT4(255, 255, 255, 255));
	//skydome->color.x = -1.0f;
}

void GameScenes::Update()
{
	StaticCameraPos::Instance()->position = DirectX::XMFLOAT3(0, 0, 0);
	StaticCameraPos::Instance()->target = DirectX::XMFLOAT3(0.01f, 0.01f, 1);

	model->position = XMFLOAT3(0, 1, 0);
	//Sea->position = XMFLOAT3(100, -2, 0);
	soko->position = XMFLOAT3(0, 0, 0);
}

void GameScenes::Draw()
{
	Drawobject3d(model);
	//Drawobject3d(soko);
	//Drawobject3d(Sea);
	Drawobject3d(soko);
	Drawobject3d(skydome);
	//Drawobject3d(squere);
}

void GameScenes::DepthDraw()
{
	DepthDrawobject3d(soko);
	DepthDrawobject3d(model);
	//DepthDrawobject3d(squere);
}

void GameScenes::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void GameScenes::ShadowDraw()
{
	ShadowDepthDrawobject3d(soko);
	ShadowDepthDrawobject3d(model);
}

void GameScenes::BloomDraw()
{
	//Drawobject3d(soko);
	//Drawobject3d(model);
	Drawobject3d(squere);
}

void GameScenes::ChangeScene(NowScene s)
{

}
