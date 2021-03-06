#pragma once
#include "FPS.h"
#include "DXAudio.h"
#include "3DObject.h"
#include "ease.h"
using namespace Ease;
#include <string.h>
//てすと
#include "PrimitiveFunc.h"

//げーむてすと
#include "GameScenes.h"
#include "GameCamera.h"

class GameManager {
	DirectX3dObject *OBJECT;
	TexManager *TEX;
	Camera *Cam;

	//ライトのテスト
	LightGroup *lightGroup = nullptr;
	//ライト場所用
	Model BoxModel[3];
	Object3d *TesObj[3];

	
public:
	GameManager(DirectX3dObject *OBJECT, TexManager *TEX, Window *Win, PipelineManager *Pileline, Camera *Cam);
	~GameManager() {
	};

	//げーむ
	GameScenes game;

	void Init();
	void Update();
	void Draw();
	void DepthDraw();
	void DOFDepthDraw();
	void ShadowDraw();
	void BloomDraw();
	void BloomDepthDraw();
};

