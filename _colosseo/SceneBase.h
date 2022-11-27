#pragma once
#include <string>
#include "SceneCommon.h"

// シーン基底
class SceneBase
{
public:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	enum SCENE_ID {
		TITLE,
		GAME
	};

protected:
	SCENE_ID m_ThisScene;
	SCENE_ID m_NextScene;
	bool m_IsNext;
	int m_WaitTimer;

public:
	SceneBase() {
		m_ThisScene = SCENE_ID::TITLE;
		m_NextScene = SCENE_ID::GAME;
		m_IsNext = false;
		m_WaitTimer = -1; // -1の時は次シーンに移行しない
	}
	virtual void Initialize(SceneCommon* _Common) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DepthDraw() = 0;
	virtual void DOFDepthDraw() = 0;
	virtual void BackDraw() = 0;
	virtual void ShadowDraw() = 0;
	virtual void BloomDraw() = 0;
	virtual void BloomDepthDraw() = 0;
	bool GetIsNext() { return m_IsNext; }
	SCENE_ID& GetNextScene() { return m_NextScene; }

private:
	void WaitUpdate() {
		if (m_WaitTimer > 0) {
			m_WaitTimer--;
		}
		if (m_WaitTimer == 0) {
			m_IsNext = true;
			m_WaitTimer = -1;
		}
	}

public:
	SceneCommon* Common = nullptr;
};

