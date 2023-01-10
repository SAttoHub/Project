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

	SceneBase::SCENE_ID m_NowScene;		// ���݂̃V�[���ԍ�
	SceneBase::SCENE_ID m_NextScene;	// ���J�ڂ���Ƃ��̃V�[���ԍ�
	
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

