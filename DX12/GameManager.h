#pragma once
#include "ParticleManager.h"
#include "Sprite.h"
#include "FPS.h"
#include "DXAudio.h"
#include "3DObject.h"
#include "ParticleEmitter.h"
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
	Sprite *SP;
	ParticleManager *PARTICLE;
	TexManager *TEX;
	Camera *Cam;


	ParticleManager Par;
	ParticleEmitter Emit;


	XMFLOAT3 DebugCamPos;

	//ライトのテスト
	LightGroup *lightGroup = nullptr;
	//ライト場所用
	Model BoxModel[3];
	Object3d *TesObj[3];

	//げーむ
	GameScenes game;
public:
	GameManager(DirectX3dObject *OBJECT, Sprite *SP, TexManager *TEX, Window *Win, PipelineManager *Pileline, Camera *Cam);
	~GameManager() {
	};


	void Init();
	void Update();
	void Draw();
	void DepthDraw();
	void ShadowDraw();

	double InCubic(double t) {
		return t * t * t;
	}
	double InSine(double t) {
		return 1.0 - cos((t * M_PI) / 2.0);
	}
};

