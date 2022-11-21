#include "GameUIsManager.h"

EnemyParamUI GameUI::m_En_Param_UI;
PlayerParamUI GameUI::m_Pl_Param_UI;

GameUI::GameUI()
{
	m_En_Param_UI.SetRefPoint(m_En_Param_UI_Ref_Point);
}

GameUI::~GameUI()
{
}

void GameUI::Draw()
{
	m_En_Param_UI.Draw();
	m_Pl_Param_UI.Draw();
}
