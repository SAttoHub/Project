#include "Audiences.h"

void Audiences::AudienceSummonAndDelete()
{
	if (Hoge == 1) {
		if (m_Audience.size() > 0) {
			while (1) {
				int Index = GetRand(0, int(m_Audience.size()) - 1);
				if (!(m_Audience[Index].GetAction() != AUD_ActionType::EXIT && m_Audience[Index].GetAction() != AUD_ActionType::ADMISSION)) continue;
				m_Audience[Index].ExitTrigger();
				Hoge *= -1;
				break;
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

Audiences::Audiences()
{
	m_Audience.reserve(MAX_AUDIENCE);
	Timer = 0;
	Hoge = 1;

	Tex = TexManager::LoadTexture("Resource/image/Chara/CharaAudSheet.png");
}

Audiences::~Audiences()
{
}

void Audiences::Init(Map* map)
{
	pMap = map;
	
	int modelData = LoadModelOBJ("Charactor", "mob");
	model = DirectX3dObject::CreateInstanceObject(GetModelData(modelData),
		XMFLOAT3(-1000, -1000, -1000), FBXSHADER_INS, MAX_AUDIENCE);
	for (int i = 0; i < MAX_AUDIENCE; i++) {
		model->object[i].rotation.y = 90.0f;
		model->object[i].scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f);
	}

	model->material.texNumber = TexManager::GetColor(XMFLOAT4(0, 255, 255, 255));
	model->isBillboard = true;
	model->UseShadow = true;
	model->UseDOF = true;
}

void Audiences::Update()
{

	Timer++;
	if (Timer == 30) {
		AudienceSummonAndDelete();
	}


	for (auto &data : m_Audience) {
		data.Update();
	}
	if (Input::isKeyTrigger(DIK_L)) {
		m_Audience[5].ExitTrigger();
	}
	for (int i = 0; i < m_Audience.size(); i++){
		model->object[i].position = m_Audience[i].GetPosition();
		XMFLOAT4 UV = m_Audience[i].GetTexUV();
		model->object[i].LT_UV = XMFLOAT2(UV.x, UV.y);
		model->object[i].RB_UV = XMFLOAT2(UV.z, UV.w);
	}
	if (m_Audience.size() > 0) {
		for (int i = 0; i < m_Audience.size() - 1; i++) {
			if (m_Audience[i].GetDelete() == true) {
				m_Audience[i].DeleteModel();
				m_Audience.erase(m_Audience.begin() + i);
			}
		}
	}

	model->material.texNumber = Tex;

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
	/*for (auto &data : m_Audience) {
		data.Draw();
	}*/
	Drawobject3d(model);
}

void Audiences::ShadowDraw()
{
	/*for (auto &data : m_Audience) {
		data.ShadowDraw();
	}*/
	ShadowDepthDrawobject3d(model);
}

void Audiences::DepthDraw()
{
	/*for (auto &data : m_Audience) {
		data.DepthDraw();
	}*/
	DepthDrawobject3d(model);
}

void Audiences::DOFDepthDraw()
{
	/*for (auto &data : m_Audience) {
		data.DOFDepthDraw();
	}*/
	DOFDepthDrawobject3d(model);
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

void Audiences::DeleteAllAudience()
{
	for (auto& aud : m_Audience) {
		aud.DeleteModel();
	}
	m_Audience.clear();
	Timer = 0;
	Hoge = 0;
}
