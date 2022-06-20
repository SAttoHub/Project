#include "GameScenes.h"

void GameScenes::Initialize()
{
	Scene = NowScene::Title;
	int modelData = LoadModelOBJ("Hako", "hako");
	model = DirectX3dObject::CreateObject(GetModelData(modelData), XMFLOAT3(0, 0, 0), Sea_SHADER);
	model->scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	int modelData2 = LoadModelOBJ("floor", "flo");
	Sea = DirectX3dObject::CreateObject(GetModelData(modelData2), XMFLOAT3(0, 0, 0), Sea_SHADER);

	int modelData3 = LoadModelOBJ("soko", "floa");
	soko = DirectX3dObject::CreateObject(GetModelData(modelData3), XMFLOAT3(0, 0, 0), Sea_SHADER);

	Sea->position = XMFLOAT3(0, 0, 0);

	Sea->color.w = 0.55f;

}

void GameScenes::Update()
{
	StaticCameraPos::Instance()->position = DirectX::XMFLOAT3(0, 0, 0);
	StaticCameraPos::Instance()->target = DirectX::XMFLOAT3(0.01f, 0.01f, 1);

	model->position = XMFLOAT3(0, -2, 0);
	soko->position = XMFLOAT3(0, -2, 0);
}

void GameScenes::Draw()
{
	//Drawobject3d(model);
	//Drawobject3d(soko);
	//Drawobject3d(Sea);
	Drawobject3d(soko);


}

void GameScenes::BackDraw()
{
	DrawGraphBack(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(XMFLOAT4(30, 30, 30, 255)));
}

void GameScenes::ChangeScene(NowScene s)
{

}
