#pragma once
#include "SceneBase.h"
#include "SceneCommon.h"

class GameScene : public SceneBase
{
	//std::unique_ptr<BattleWave> m_BattleWave;
	BattleWave *m_BattleWave;
	int NowWave; // ��Wave
	int EndCount; // ���Q�[���I���J�E���g
	int Turn; // 0 : �Ȃ��@1 : �G�@2 : �v���C���[

	int ClearTex;

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

