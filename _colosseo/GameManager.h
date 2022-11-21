#pragma once
#include "FPS.h"
#include "DXAudio.h"
#include "Engine\Objects\3DObject.h"
#include "Engine\CommonMathIncludes.h"
using namespace Ease;
#include <string.h>
//�Ă���
#include "Engine\Primitive\PrimitiveFunc.h"

//���[�ނĂ���
#include "Game/GameScenes.h"
#include "Game/GameCamera.h"

class GameManager {
	DirectX3dObject *OBJECT;
	TexManager *TEX;
	Camera *Cam;

	//���C�g�̃e�X�g
	LightGroup *lightGroup = nullptr;
	//���C�g�ꏊ�p
	Model BoxModel[3];
	Object3d *TesObj[3];

	
public:
	GameManager(DirectX3dObject *OBJECT, TexManager *TEX, Window *Win, PipelineManager *Pileline, Camera *Cam);
	~GameManager() {
	};

	//���[��
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

