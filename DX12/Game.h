#pragma once
#include "Framework.h"
#include "PostEffect.h"

#include "Gaussian.h"
#include "DepthOfField.h"
#include "Depth.h"

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
	Depth *depth = nullptr; // ê[ìxâÊëúóp
public:
	void Initialize() override;
	void Update();
	void Finalize() override;

	void Run(const int iCmdShow);

	GameManager *game = nullptr;
};

