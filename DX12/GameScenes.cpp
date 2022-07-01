#include "GameScenes.h"

void GameScenes::Initialize()
{
	Scene = NowScene::Title;
	int modelData = LoadModelOBJ("Hako", "hako");
	model = DirectX3dObject::CreateObject(GetModelData(modelData), XMFLOAT3(0, 0, 0), FBXSHADER);
	model->scale = XMFLOAT3(10.0f, 10.0f, 10.0f);

	int modelData2 = LoadModelOBJ("Hako", "flo");
	Sea = DirectX3dObject::CreateObject(GetModelData(modelData2), XMFLOAT3(0, 0, 0), FBXSHADER);
	Sea->scale = XMFLOAT3(10.0f, 10.0f, 10.0f);

	int modelData3 = LoadModelOBJ("soko", "floa");
	soko = DirectX3dObject::CreateObject(GetModelData(modelData3), XMFLOAT3(0, 0, 0), FBXSHADER);
	soko->scale = XMFLOAT3(100.0f, 100.0f, 100.0f);

	Sea->position = XMFLOAT3(0, 0, 0);

	Sea->color.w = 0.55f;

}

void GameScenes::Update()
{
	StaticCameraPos::Instance()->position = DirectX::XMFLOAT3(0, 0, 0);
	StaticCameraPos::Instance()->target = DirectX::XMFLOAT3(0.01f, 0.01f, 1);

	model->position = XMFLOAT3(0, 50, -50);
	Sea->position = XMFLOAT3(100, -2, 0);
	soko->position = XMFLOAT3(-50, -10, -50);
}

void GameScenes::Draw()
{
	Drawobject3d(model);
	//Drawobject3d(soko);
	Drawobject3d(Sea);
	Drawobject3d(soko);


}

void GameScenes::DepthDraw()
{
	DepthDrawobject3d(model);
	DepthDrawobject3d(Sea);
	DepthDrawobject3d(soko);
}

void GameScenes::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void GameScenes::ShadowDraw()
{
	ShadowDepthDrawobject3d(model);
	ShadowDepthDrawobject3d(Sea);
	ShadowDepthDrawobject3d(soko);
}

void GameScenes::ChangeScene(NowScene s)
{

}
