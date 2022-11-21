#pragma once
#include "EnemyParamUI.h"
#include "PlayerParamUI.h"
#include "..\Window.h"

// ÉQÅ[ÉÄíÜUIìùäá
class GameUI {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	static EnemyParamUI m_En_Param_UI;
	static PlayerParamUI m_Pl_Param_UI;
	const XMFLOAT2 m_En_Param_UI_Ref_Point = XMFLOAT2(WINDOW_WIDTH - 200.0f, 0.0f);

public:
	GameUI();
	~GameUI();

	static EnemyParamUI* GetEnemyParamUI() { return &m_En_Param_UI; }
	static PlayerParamUI* GetPlayerParamUI() { return &m_Pl_Param_UI; }

	void Draw();
};