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

class Game : public Framework
{
private:
	int RDot;
	SpriteData CenterDot;
	Model skydomeData;
	Object3d *skydome = nullptr;

	PostEffect *postEffect = nullptr;
	Gaussian *GaussianEffectX = nullptr;
	Gaussian *GaussianEffectY = nullptr;
	Gaussian *GaussianEffectX2 = nullptr;
	Gaussian *GaussianEffectY2 = nullptr;
	DepthOfField *DOF = nullptr;
	Depth *depth = nullptr; // �[�x�摜�p
	Depth2 *depth2 = nullptr; // �[�x�摜�p
	//�u���[��
	Bloom *bloom = nullptr;
	Gaussian *GaussianEffectXBloom = nullptr;
	Gaussian *GaussianEffectYBloom = nullptr;

	//�V���h�E
	ShadowMapLight *Shadow_Map_Light;
	ShadowMapping *shadowMapping = nullptr;

	bool BloomFlag;
	bool DOFFlag;
	XMFLOAT3 DOF_State;
public:
	void Initialize() override;
	void Update();
	void Finalize() override;

	void Run(const int iCmdShow);

	GameManager *game = nullptr;
};

