#pragma once
#include <vector>
#include <memory>
#include "SceneBase.h"
#include "Singleton.h"
#include "SceneCommon.h"
#include "Engine/Common/Containers/MyFList.h"


class SceneManager : public Singleton <SceneManager>
{
	friend class Singleton <SceneManager>;
private:
	My_F_List<std::shared_ptr<SceneBase>> m_Scenes;
	SceneCommon Common;
private:

	SceneBase::SCENE_ID m_NowScene;		// 現在のシーン番号
	SceneBase::SCENE_ID m_NextScene;	// 次遷移するときのシーン番号
	
public:
	//SceneManager() {};

	void Initialize();
	void Update();
	void Draw();
	void DepthDraw();
	void DOFDepthDraw();
	void BackDraw();
	void ShadowDraw();
	void BloomDraw();
	void BloomDepthDraw();
};

