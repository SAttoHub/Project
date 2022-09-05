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
	// �K�E�X�ڂ����p
	Gaussian *GaussianEffectX = nullptr;
	Gaussian *GaussianEffectY = nullptr;
	Gaussian *GaussianEffectX2 = nullptr;
	Gaussian *GaussianEffectY2 = nullptr;
	Gaussian *GaussianEffectX_b = nullptr;
	Gaussian *GaussianEffectY_b = nullptr;
	DepthOfField *DOF = nullptr;

	bool DOFFlag;
	//�u���[��
	Bloom *bloom = nullptr;
	bool BloomFlag;
	//�V���h�E
	ShadowMapLight *Shadow_Map_Light;
	ShadowMapping *shadowMapping = nullptr;

	//�r�l�b�g
	Vignette *vignette = nullptr;

	//�A�E�g���C��
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

