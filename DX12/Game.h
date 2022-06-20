#pragma once
#include "Framework.h"
#include "PostEffect.h"

#include "Gaussian.h"

class Game : public Framework
{
private:
	int RDot;
	SpriteData CenterDot;
	Model skydomeData;
	Object3d *skydome = nullptr;

	PostEffect *postEffect = nullptr;
	Gaussian *GaussianEffect = nullptr;
public:
	void Initialize() override;
	void Update();
	void Finalize() override;

	void Run(const int iCmdShow);

	GameManager *game = nullptr;
};

