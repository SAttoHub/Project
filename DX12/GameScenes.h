#pragma once
#include "Singleton.h"
#include "StaticCameraPos.h"

#include "PrimitiveFunc.h"
#include "DrawStrings.h"
#include "ColorFuncs.h"
#include "ColorDefines.h"
#include "3DObject.h"
#include "ModelManager.h"
#include "ExtendedRandom.h"

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

	Object3d *soko;
	Object3d *squere;
	Object3d *skydome;

	Object3d *testStage;
	Object3d *saku;
	Object3d *saku2;
	Object3d *saku3;

	Object3d *saku4;
	bool bloomFlag1;
	Object3d *saku5;
	bool bloomFlag2;

public:
	void Initialize();
	void Update();
	void Draw();
	void DepthDraw();
	void DOFDepthDraw();
	void BackDraw();
	void ShadowDraw();
	void BloomDraw();
	void BloomDepthDraw();

	void ChangeScene(NowScene s);
};

