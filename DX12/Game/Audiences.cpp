#include "Audiences.h"

Audiences::Audiences()
{
	m_Audience.reserve(MAX_AUDIENCE);
	Timer = 0;
	Hoge = 1;
}

Audiences::~Audiences()
{
}

void Audiences::Init(Map* map)
{
	pMap = map;
}

void Audiences::Update()
{

	Timer++;
	if (Timer == 30) {
		if (Hoge == 1) {
			if (m_Audience.size() > 0) {
				while (1) {
					int Index = GetRand(0, int(m_Audience.size()) - 1);
					if (m_Audience[Index].GetAction() != AUD_ActionType::EXIT && m_Audience[Index].GetAction() != AUD_ActionType::ADMISSION) {
						m_Audience[Index].ExitTrigger();
						Hoge *= -1;
						break;
					}
				}
			}
			Timer = 0;
		}
		else {
			SummonAudience2(1);
			Timer = 0;
			Hoge *= -1;
		}
	}


	for (auto &data : m_Audience) {
		data.Update();
	}
	if (Input::isKeyTrigger(DIK_L)) {
		m_Audience[5].ExitTrigger();
	}
	for (int i = 0; i < m_Audience.size() - 1; i++) {
		if (m_Audience[i].GetDelete() == true) {
			m_Audience.erase(m_Audience.begin() + i);
		}
	}

	/*ImGui::SetNextWindowPos(ImVec2(0, 60), 1);
	ImGui::SetNextWindowSize(ImVec2(500, 500), 1);
	ImGui::Begin("Config2");
	ImGui::SliderFloat("1段目　距離", &Audience::Step1.x, 40.0f, 200.0f);
	ImGui::SliderFloat("2段目　距離", &Audience::Step2.x, 40.0f, 200.0f);
	ImGui::SliderFloat("3段目　距離", &Audience::Step3.x, 40.0f, 200.0f);
	ImGui::SliderFloat("4段目　距離", &Audience::Step4.x, 40.0f, 200.0f);
	ImGui::SliderFloat("1段目　高さ", &Audience::Step1.y, 0.0f, 200.0f);
	ImGui::SliderFloat("2段目　高さ", &Audience::Step2.y, 0.0f, 200.0f);
	ImGui::SliderFloat("3段目　高さ", &Audience::Step3.y, 0.0f, 200.0f);
	ImGui::SliderFloat("4段目　高さ", &Audience::Step4.y, 0.0f, 200.0f);
	ImGui::End();*/
}

void Audiences::Draw()
{
	for (auto &data : m_Audience) {
		data.Draw();
	}
}

void Audiences::ShadowDraw()
{
	for (auto &data : m_Audience) {
		data.ShadowDraw();
	}
}

void Audiences::DepthDraw()
{
	for (auto &data : m_Audience) {
		data.DepthDraw();
	}
}

void Audiences::DOFDepthDraw()
{
	for (auto &data : m_Audience) {
		data.DOFDepthDraw();
	}
}

void Audiences::SummonAudience(int Count)
{
	for (int i = 0; i < Count; i++) {
		// 既に最大数存在するなら生成しない
		if (int(m_Audience.size()) >= MAX_AUDIENCE) {
			return;
		}
		// 召喚する段とインデックスを決定
		int CoStep = 1;
		int CoIndex = 0;
		while (1) {
			CoStep = GetRand(1, 5);
			CoIndex = GetRandIndex();
			bool LoopExit = true;
			for (auto &data : m_Audience) {
				if (data.GetStep() == CoStep && data.GetIndex() == CoIndex) {
					LoopExit = false;
				}
			}
			if (LoopExit == true)break;
		}

		// Audience追加
		m_Audience.emplace_back();
		int Index = int(m_Audience.size()) - 1;
		m_Audience[Index].SetMap(pMap);
		m_Audience[Index].SetPosition(CoStep, CoIndex);
		m_Audience[Index].SetDir();
	}
}

void Audiences::SummonAudience2(int Count)
{
	for (int i = 0; i < Count; i++) {
		// 既に最大数存在するなら生成しない
		if (int(m_Audience.size()) >= MAX_AUDIENCE) {
			return;
		}
		// 召喚する段とインデックスを決定
		int CoStep = 1;
		int CoIndex = 0;
		while (1) {
			CoStep = GetRand(1, 5);
			CoIndex = GetRandIndex();
			bool LoopExit = true;
			for (auto &data : m_Audience) {
				if (data.GetStep() == CoStep && data.GetIndex() == CoIndex) {
					LoopExit = false;
				}
			}
			if (LoopExit == true)break;
		}

		// Audience追加
		m_Audience.emplace_back();
		int Index = int(m_Audience.size()) - 1;
		m_Audience[Index].SetMap(pMap);
		m_Audience[Index].SetPosition(CoStep, CoIndex);
		m_Audience[Index].SetDir();
		m_Audience[Index].AdmissionTrigger();
	}
}

int Audiences::GetRandIndex()
{
	int Index = GetRand(5, 25);
	int Index2 = GetRand(0, 3);
	Index = 30 * Index2 + Index;
	return Index;
}
