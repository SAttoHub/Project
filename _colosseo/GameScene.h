#pragma once
#include "SceneBase.h"
#include "SceneCommon.h"

class GameScene : public SceneBase
{
	//std::unique_ptr<BattleWave> m_BattleWave;
	BattleWave *m_BattleWave;
	int NowWave; // 仮Wave
	int EndCount; // 仮ゲーム終了カウント
	int Turn; // 0 : なし　1 : 敵　2 : プレイヤー

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

