#pragma once
#include "../Engine/CommonMathIncludes.h"

class Enemy;
class Player;

class ActionEffectBase {
protected:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

	// カメラ視点用のターゲット座標
	XMFLOAT3 m_Target;

	int m_Timer = 0;
	int m_EndTime = 0;

	// タイマーを進める
	void TimerUpdate();
public:

	virtual void Start(int _Timer, int _EndTime) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// 終了フラグ
	bool m_isEnd = false;
};