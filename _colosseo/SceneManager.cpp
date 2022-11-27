#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"

SceneManager::SceneManager()
{
}

void SceneManager::Initialize()
{
	Common.Initialize();

	m_Scenes.emplace_back(std::make_shared<TitleScene>());
	m_Scenes.emplace_back(std::make_shared<GameScene>());
	m_NowScene = SceneBase::SCENE_ID::TITLE;
	m_NextScene = SceneBase::SCENE_ID::GAME;

	m_Scenes[m_NowScene]->Initialize(&Common);
}

void SceneManager::Update()
{
	m_Scenes[m_NowScene]->Update();

	if (m_Scenes[m_NowScene]->GetIsNext()) {

		m_NowScene = m_Scenes[m_NowScene]->GetNextScene();

		m_Scenes[m_NowScene]->Initialize(&Common);
	}
}

void SceneManager::Draw()
{
	m_Scenes[m_NowScene]->Draw();
}

void SceneManager::DepthDraw()
{
	m_Scenes[m_NowScene]->DepthDraw();
}

void SceneManager::DOFDepthDraw()
{
	m_Scenes[m_NowScene]->DOFDepthDraw();
}

void SceneManager::BackDraw()
{
	m_Scenes[m_NowScene]->BackDraw();
}

void SceneManager::ShadowDraw()
{
	m_Scenes[m_NowScene]->ShadowDraw();
}

void SceneManager::BloomDraw()
{
	m_Scenes[m_NowScene]->BloomDraw();
}

void SceneManager::BloomDepthDraw()
{
	m_Scenes[m_NowScene]->BloomDepthDraw();
}
