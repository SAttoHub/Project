#pragma once
#include "SceneBase.h"
#include "SceneCommon.h"

#include "LoadJsonManager.h"

class TitleScene :   public SceneBase
{
private:
	SceneCommon* Common;

	int TitleTex;

	std::vector<Object3d*> test;
public:
	void Initialize(SceneCommon* _Common) override;
	void Update() override;
	void Draw() override;
	void DepthDraw() override;
	void DOFDepthDraw() override;
	void BackDraw() override;
	void ShadowDraw() override;
	void BloomDraw() override;
	void BloomDepthDraw() override;
};

