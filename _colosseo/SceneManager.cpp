#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "StageSelectScene.h"


void SceneManager::Initialize()
{
	Common.Initialize();

	m_Scenes.emplace_front(std::make_shared<TitleScene>());
	//m_Scenes.emplace_back(std::make_shared<GameScene>());
	m_Scenes.emplace_after(std::next(m_Scenes.begin(), 0), std::make_shared<StageSelectScene>());
	m_Scenes.emplace_after(std::next(m_Scenes.begin(), 1), std::make_shared<GameScene>());
	
	m_NowScene = SceneBase::SCENE_ID::TITLE;
	m_NextScene = SceneBase::SCENE_ID::GAME;

	m_Scenes[m_NowScene]->get()->Initialize(&Common);
}

void SceneManager::Update()
{
	m_Scenes[m_NowScene]->get()->Update();

	if (m_Scenes[m_NowScene]->get()->GetIsNext()) {

		m_NowScene = m_Scenes[m_NowScene]->get()->GetNextScene();

		m_Scenes[m_NowScene]->get()->Initialize(&Common);
	}
}

void SceneManager::Draw()
{
	m_Scenes[m_NowScene]->get()->Draw();
}

void SceneManager::DepthDraw()
{
	m_Scenes[m_NowScene]->get()->DepthDraw();
}

void SceneManager::DOFDepthDraw()
{
	m_Scenes[m_NowScene]->get()->DOFDepthDraw();
}

void SceneManager::BackDraw()
{
	m_Scenes[m_NowScene]->get()->BackDraw();
}

void SceneManager::ShadowDraw()
{
	m_Scenes[m_NowScene]->get()->ShadowDraw();
}

void SceneManager::BloomDraw()
{
	m_Scenes[m_NowScene]->get()->BloomDraw();
}

void SceneManager::BloomDepthDraw()
{
	m_Scenes[m_NowScene]->get()->BloomDepthDraw();
}
