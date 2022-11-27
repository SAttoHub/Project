#pragma once
#include "SceneBase.h"
#include "SceneCommon.h"

class TitleScene :   public SceneBase
{
private:
	SceneCommon* Common;

	int TitleTex;
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

