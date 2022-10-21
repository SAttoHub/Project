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
#include "OutLine.h"

class Game : public Framework
{
private:
	// ガウスぼかし用
	std::unique_ptr<Gaussian> GaussianEffectX = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectY = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectX2 = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectY2 = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectX_b = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectY_b = nullptr;
	std::unique_ptr<DepthOfField> DOF = nullptr;

	bool DOFFlag;
	//ブルーム
	std::unique_ptr<Bloom> bloom = nullptr;
	bool BloomFlag;
	//シャドウ
	std::unique_ptr<ShadowMapLight> Shadow_Map_Light = nullptr;
	std::unique_ptr<ShadowMapping> shadowMapping = nullptr;

	//ビネット
	std::unique_ptr<Vignette> vignette = nullptr;

	//アウトライン
	/*bool UseOutLine;
	OutLine *outLine = nullptr;*/



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

