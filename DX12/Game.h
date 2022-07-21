#pragma once
#include "Framework.h"
#include "PostEffect.h"

#include "Gaussian.h"
#include "DepthOfField.h"
#include "Depth.h"
#include "Depth2.h"
#include "Bloom.h"
#include "ShadowMapLight.h"
#include "ShadowMapping.h"
#include "Vignette.h"

class Game : public Framework
{
private:

	// ガウスぼかし用
	Gaussian *GaussianEffectX = nullptr;
	Gaussian *GaussianEffectY = nullptr;
	DepthOfField *DOF = nullptr;

	bool DOFFlag;
	//ブルーム
	Bloom *bloom = nullptr;
	bool BloomFlag;
	//シャドウ
	ShadowMapLight *Shadow_Map_Light;
	ShadowMapping *shadowMapping = nullptr;

	//ビネット
	Vignette *vignette = nullptr;



	float InterpSize;
	float Focus;
	float FocusSize;
	bool UseFlag;

	bool ShadowMapUse;

	float VignetteInfluence;
	bool UseVignette;
public:
	void Initialize() override;
	void Update();
	void Finalize() override;

	void Run(const int iCmdShow);

	GameManager *game = nullptr;
};

