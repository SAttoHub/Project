#include "GameScenes.h"

void GameScenes::Initialize()
{
	Scene = NowScene::Title;
	int modelData = LoadModelOBJ("BURI", "hako");
	model1 = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(-30, 20, 0), FBXSHADER);
	model1->scale = XMFLOAT3(10, 10, 10);
	model2 = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 20, 0), FBXSHADER);
	model2->scale = XMFLOAT3(10, 10, 10);
	model3 = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(30, 20, 0), FBXSHADER);
	model3->scale = XMFLOAT3(10, 10, 10);



	int modelData3 = LoadModelOBJ("soko", "floa");
	soko = DirectX3dObject::CreateObject(GetModelData(modelData3), XMFLOAT3(0, 0, 0), Sea_SHADER);
	soko->scale = XMFLOAT3(100.0f, 100.0f, 100.0f);
	soko->material.texNumber = TexManager::GetColor(XMFLOAT4(10, 150, 150, 255));
	//soko->material.texNumber = TexManager::GetColor(Color_Light_Blue);

	int skydomeDataNum = LoadModelOBJ("skydome", "skydome");
	skydome = DirectX3dObject::CreateObject(GetModelData(skydomeDataNum), XMFLOAT3(0, 0, 0), FBXSHADER);
	skydome->scale = XMFLOAT3(3.0f, 3.0f, 3.0f);
	//skydome->color.x = -1.0f;

	BloomFlag1 = true;
	BloomFlag2 = false;
	BloomFlag3 = false;
}

void GameScenes::Update()
{
	StaticCameraPos::Instance()->position = DirectX::XMFLOAT3(0, 0, 0);
	StaticCameraPos::Instance()->target = DirectX::XMFLOAT3(0.01f, 0.01f, 1);

	soko->position = XMFLOAT3(0, 0, 0);
}

void GameScenes::Draw()
{
	/*for (int i = 0; i < 50; i++) {
		Drawobject3d(model[i]);
	}*/
	Drawobject3d(model1);
	Drawobject3d(model2);
	Drawobject3d(model3);
	//Drawobject3d(soko);
	//Drawobject3d(Sea);
	Drawobject3d(soko);
	Drawobject3d(skydome);
	//Drawobject3d(squere);
}

void GameScenes::DepthDraw()
{
	DepthDrawobject3d(soko);
	DepthDrawobject3d(model1);
	DepthDrawobject3d(model2);
	DepthDrawobject3d(model3);
	/*for (int i = 0; i < 50; i++) {
		DepthDrawobject3d(model[i]);
	}*/
	//DepthDrawobject3d(squere);
}

void GameScenes::DOFDepthDraw()
{
	DOFDepthDrawobject3d(soko);
	DOFDepthDrawobject3d(model1);
	DOFDepthDrawobject3d(model2);
	DOFDepthDrawobject3d(model3);
}

void GameScenes::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void GameScenes::ShadowDraw()
{
	ShadowDepthDrawobject3d(soko);
	ShadowDepthDrawobject3d(model1);
	ShadowDepthDrawobject3d(model2);
	ShadowDepthDrawobject3d(model3);
	/*for (int i = 0; i < 50; i++) {
		ShadowDepthDrawobject3d(model[i]);
	}*/
}

void GameScenes::BloomDraw()
{
	//Drawobject3d(soko);
	//Drawobject3d(model);
	if (BloomFlag1) {
		Drawobject3d(model1);
	}
	if (BloomFlag2) {
		Drawobject3d(model2);
	}
	if (BloomFlag3) {
		Drawobject3d(model3);
	}
}

void GameScenes::BloomDepthDraw()
{
	if (BloomFlag1) {
		DepthDrawobject3d(model1);
	}
	if (BloomFlag2) {
		DepthDrawobject3d(model2);
	}
	if (BloomFlag3) {
		DepthDrawobject3d(model3);
	}
}

void GameScenes::ChangeScene(NowScene s)
{

}
