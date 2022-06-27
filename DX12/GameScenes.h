#pragma once
#include "Singleton.h"
#include "StaticCameraPos.h"

#include "PrimitiveFunc.h"
#include "DrawStrings.h"
#include "ColorFuncs.h"
#include "ColorDefines.h"
#include "3DObject.h"
#include "ModelManager.h"

enum class NowScene {
	Title,
	StageCelect,
	Game,
	Result,
	Tutorial
};
class GameScenes : public Singleton<GameScenes>
{
	friend class Singleton<GameScenes>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	NowScene Scene;

	Object3d *model;
	Object3d *Sea;
	Object3d *soko;


public:
	void Initialize();
	void Update();
	void Draw();
	void DepthDraw();
	void BackDraw();

	void ChangeScene(NowScene s);
};

