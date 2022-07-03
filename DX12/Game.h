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
	int RDot;
	SpriteData CenterDot;
	Model skydomeData;
	Object3d *skydome = nullptr;

	PostEffect *postEffect = nullptr;
	PostEffect *postEffect2 = nullptr;
	Gaussian *GaussianEffectX = nullptr;
	Gaussian *GaussianEffectY = nullptr;
	Gaussian *GaussianEffectX2 = nullptr;
	Gaussian *GaussianEffectY2 = nullptr;
	DepthOfField *DOF = nullptr;
	Depth *depth = nullptr; // �[�x�摜�p(DOF)
	Depth2 *depth2 = nullptr; // �[�x�摜�p
	Depth2 *depth3 = nullptr; // �[�x�摜�p(Bloom)

	bool DOFFlag;
	//�u���[��
	Bloom *bloom = nullptr;
	Gaussian *GaussianEffectXBloom = nullptr;
	Gaussian *GaussianEffectYBloom = nullptr;
	bool BloomFlag;
	//�V���h�E
	ShadowMapLight *Shadow_Map_Light;
	ShadowMapping *shadowMapping = nullptr;

	//�r�l�b�g
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

