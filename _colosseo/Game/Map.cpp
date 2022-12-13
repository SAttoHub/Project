#include "Map.h"
#include "..\Collision2.h"

Map::Map()
{
	/*int modelData = LoadModelOBJ("Ground3", "stage");
	m_StageModel = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel->rotation.y = 90.0f;
	m_StageModel->scale.x = 7.5f / 4.0f;
	m_StageModel->scale.y = 7.5f / 4.0f;
	m_StageModel->scale.z = 7.5f / 4.0f;
	m_StageModel->position.x = 32.0f * 11.0f / 4.0f;
	m_StageModel->position.z = -32.0f / 4.0f;*/
	/*int modelData2 = LoadModelOBJ("StageSaku", "stageSaku");
	m_StageModel_saku = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel_saku->rotation.y = 90.0f;
	m_StageModel_saku->scale.x = 7.5f / 4.0f;
	m_StageModel_saku->scale.y = 7.5f / 4.0f;
	m_StageModel_saku->scale.z = 7.5f / 4.0f;
	m_StageModel_saku->position.x = 32.0f * 11.0f / 4.0f;
	m_StageModel_saku->position.z = -32.0f / 4.0f;*/

	/*int modelData = LoadModelOBJ("Ground_Data", "stage");
	m_StageModel = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel->rotation.y = 90.0f;
	m_StageModel->scale.x = 8.0f;
	m_StageModel->scale.y = 8.0f;
	m_StageModel->scale.z = 8.0f;
	m_StageModel->position.x = 32.0f * 10.0f / 4.0f;

	int modelData2 = LoadModelOBJ("Fence_Data", "stage");
	m_StageModel_saku = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel_saku->rotation.y = 90.0f;
	m_StageModel_saku->scale.x = 8.0f;
	m_StageModel_saku->scale.y = 8.0f;
	m_StageModel_saku->scale.z = 8.0f;
	m_StageModel_saku->position.x = 32.0f * 10.0f / 4.0f;


	int modelData3 = LoadModelOBJ("Tree_Data", "stage");
	m_StageModel_tree = DirectX3dObject::CreateObject(GetModelData(modelData3),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel_tree->rotation.y = 90.0f;
	m_StageModel_tree->scale.x = 8.0f;
	m_StageModel_tree->scale.y = 8.0f;
	m_StageModel_tree->scale.z = 8.0f;
	m_StageModel_tree->position.x = 32.0f * 10.0f / 4.0f;*/

	isHitChip = false;


	int modelData = LoadModelOBJ("Colosseum", "stage");
	m_StageModel = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel->rotation.y = 90.0f;
	m_StageModel->scale.x = 8.0f;
	m_StageModel->scale.y = 8.0f;
	m_StageModel->scale.z = 8.0f;
	m_StageModel->position.x = 32.0f * (float(m_MAP_SIZE_X) / 2.0f) / 4.0f;
	m_StageModel->position.z = 32.0f * (float(m_MAP_SIZE_Z) / 2.0f) / 4.0f;
	m_StageModel->UseShadow = true;
	m_StageModel->UseDOF = true;

	int modelData2 = LoadModelOBJ("Fence", "stage");
	m_StageModel_saku = DirectX3dObject::CreateObject(GetModelData(modelData2),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel_saku->rotation.y = 90.0f;
	m_StageModel_saku->scale.x = 8.0f;
	m_StageModel_saku->scale.y = 8.0f;
	m_StageModel_saku->scale.z = 8.0f;
	m_StageModel_saku->position.x = 32.0f * (float(m_MAP_SIZE_X) / 2.0f) / 4.0f;
	m_StageModel_saku->position.z = 32.0f * (float(m_MAP_SIZE_Z) / 2.0f) / 4.0f;
	m_StageModel_saku->UseShadow = true;
	m_StageModel_saku->UseDOF = true;

	int modelDataSand = LoadModelOBJ("sandfloor", "stage");
	m_StageModel_floor = DirectX3dObject::CreateObject(GetModelData(modelDataSand),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	m_StageModel_floor->rotation.y = 90.0f;
	m_StageModel_floor->scale.x = 8.0f;
	m_StageModel_floor->scale.y = 8.0f;
	m_StageModel_floor->scale.z = 8.0f;
	m_StageModel_floor->position.x = 32.0f * (float(m_MAP_SIZE_X) / 2.0f) / 4.0f;
	m_StageModel_floor->position.y = -0.5f;
	m_StageModel_floor->position.z = 32.0f * (float(m_MAP_SIZE_Z) / 2.0f) / 4.0f;
	m_StageModel_floor->UseShadow = true;
	m_StageModel_floor->UseDOF = true;

	// Pillar
	{
		XMFLOAT3 Center = { 32.0f * (float(m_MAP_SIZE_X) / 2.0f) / 4.0f, 0.0f,  32.0f * (float(m_MAP_SIZE_Z) / 2.0f) / 4.0f };
		for (int i = 0; i < PILLAR_MAX; i++) {
			m_PillarDatas[i].Pos = Center;
			m_PillarDatas[i].Rot = XMFLOAT3(0.0f, 90.0f, 0.0f);
			m_PillarDatas[i].Scale = XMFLOAT3(6.0f, 6.0f, 6.0f);
		}
		m_PillarDatas[0].Pos = XMFLOAT3(Center.x - 20.0f, Center.y, Center.z + 20.0f);
		m_PillarDatas[1].Pos = XMFLOAT3(Center.x - 20.0f, Center.y, Center.z - 20.0f);
		m_PillarDatas[2].Pos = XMFLOAT3(Center.x + 20.0f, Center.y, Center.z + 20.0f);
		m_PillarDatas[3].Pos = XMFLOAT3(Center.x + 20.0f, Center.y, Center.z - 20.0f);

		int modelDataPillar = LoadModelOBJ("pillar", "stage");
		m_Pillar = DirectX3dObject::CreateInstanceObject(GetModelData(modelDataPillar),
			XMFLOAT3(0, 0, 0), FBXSHADER_INS, PILLAR_MAX, PILLAR_MAX);
		m_Pillar->UseShadow = true;
		m_Pillar->UseDOF = true;

		for (int i = 0; i < PILLAR_MAX; i++) {
			m_Pillar->object[i].position = m_PillarDatas[i].Pos;
			m_Pillar->object[i].rotation = m_PillarDatas[i].Rot;
			m_Pillar->object[i].scale = m_PillarDatas[i].Scale;
		}
	}
	//


	XMFLOAT3 hoge = { 32.0f * (float(m_MAP_SIZE_X) / 2.0f) / 4.0f, 0.0f,  32.0f * (float(m_MAP_SIZE_Z) / 2.0f) / 4.0f };
	for (int i = 0; i < TORCH_MAX; i++) {
		m_TorchDatas[i].Pos = hoge;
		m_TorchDatas[i].Rot = XMFLOAT3(0.0f, 90.0f, 0.0f);
		m_TorchDatas[i].Scale = XMFLOAT3(3.0f, 3.0f, 3.0f);
	}
	m_TorchDatas[0].Pos = XMFLOAT3(hoge.x - 70.0f, hoge.y, hoge.z + 10.0f);
	m_TorchDatas[1].Pos = XMFLOAT3(hoge.x - 70.0f, hoge.y, hoge.z - 10.0f);
	m_TorchDatas[2].Pos = XMFLOAT3(hoge.x + 70.0f, hoge.y, hoge.z + 10.0f);
	m_TorchDatas[3].Pos = XMFLOAT3(hoge.x + 70.0f, hoge.y, hoge.z - 10.0f);

	m_TorchDatas[4].Pos = XMFLOAT3(hoge.x + 8.0f, 18.45f, hoge.z - 91.0f);
	m_TorchDatas[5].Pos = XMFLOAT3(hoge.x - 8.0f, 18.45f, hoge.z - 91.0f);
	m_TorchDatas[6].Pos = XMFLOAT3(hoge.x + 8.0f, 18.45f, hoge.z + 91.0f);
	m_TorchDatas[7].Pos = XMFLOAT3(hoge.x - 8.0f, 18.45f, hoge.z + 91.0f);

	for (int i = 0; i < TORCH_MAX; i++) {
		m_TorchBloomDatas[i].Pos = m_TorchDatas[i].Pos;
		m_TorchBloomDatas[i].Pos.y += 10.0f;
		m_TorchBloomDatas[i].Rot = m_TorchDatas[i].Rot;
		m_TorchBloomDatas[i].Scale = m_TorchDatas[i].Scale * 0.25f;
	}
	 
	int modelData3 = LoadModelOBJ("Torch3", "stage");
	torch = DirectX3dObject::CreateInstanceObject(GetModelData(modelData3),
		XMFLOAT3(0, 0, 0), FBXSHADER_INS, TORCH_MAX, TORCH_MAX);
	torch->UseShadow = true;
	torch->UseDOF = true;

	int modelData4 = LoadModelOBJ("ICO", "stage");
	torchBloom = DirectX3dObject::CreateInstanceObject(GetModelData(modelData4),
		XMFLOAT3(0, 0, 0), FBXSHADER_INS, TORCH_MAX, TORCH_MAX);
	torchBloom->UseShadow = true;
	torchBloom->UseDOF = true;

	for (int i = 0; i < TORCH_MAX; i++) {
		torch->object[i].position = m_TorchDatas[i].Pos;
		torch->object[i].rotation = m_TorchDatas[i].Rot;
		torch->object[i].scale = m_TorchDatas[i].Scale;

		torchBloom->object[i].position = m_TorchBloomDatas[i].Pos;
		torchBloom->object[i].rotation = m_TorchBloomDatas[i].Rot;
		torchBloom->object[i].scale = m_TorchBloomDatas[i].Scale;
	}

	Center = m_StageModel->position;

	int modelData5 = LoadModelOBJ("ChipGuide", "ChipGuide");
	ChipGuide = DirectX3dObject::CreateInstanceObject(GetModelData(modelData5),
		XMFLOAT3(0, 0, 0), FBXSHADER_INS_GUIDE, GUIDE_MAX, GUIDE_MAX);
	ChipGuide->UseShadow = false;
	ChipGuide->UseDOF = true;
	for (int i = 0; i < GUIDE_MAX; i++) {
		ChipGuide->object[i].scale = XMFLOAT3(3.95f, 3.95f, 3.95f);
		ChipGuide->object[i].isDraw = false;
	}
}

Map::~Map()
{
}

ChipData* Map::Get(XMINT2 Pos)
{
	for (auto &dataX : m_Data) {
		for (auto &dataY : dataX) {
			if (dataY.m_Pos.x == Pos.x && dataY.m_Pos.z == Pos.y) {
				return &dataY;
			}
		}
	}
	return nullptr;
}

void Map::Initialize()
{
	m_Data.resize(m_MAP_SIZE_X);
	for (auto &dataX : m_Data) {
		dataX.resize(m_MAP_SIZE_Z);
	}
	for (int x = 0; x < m_Data.size(); x++) {
		for (int z = 0; z < m_Data[x].size(); z++) {
			m_Data[x][z].m_Pos = XMINT3(x, 0, z);
			for (int p = 0; p < PILLAR_MAX; p++) {
				if (m_PillarDatas[p].Pos.x == float(x * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE / 2.0f) &&
					m_PillarDatas[p].Pos.z == float(z * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE / 2.0f)) {
					m_Data[x][z].Cost = 9999;
				}
			}
		}
	}
	m_Data[0][0].Cost = 9999;
	m_Data[1][0].Cost = 9999;
	m_Data[0][1].Cost = 9999;

	m_Data[13][0].Cost = 9999;
	m_Data[12][0].Cost = 9999;
	m_Data[13][1].Cost = 9999;

	m_Data[0][13].Cost = 9999;
	m_Data[0][12].Cost = 9999;
	m_Data[1][13].Cost = 9999;

	m_Data[13][13].Cost = 9999;
	m_Data[12][13].Cost = 9999;
	m_Data[13][12].Cost = 9999;

	CostTable.resize(m_MAP_SIZE_X);
	for (auto &dataX : CostTable) {
		dataX.resize(m_MAP_SIZE_Z);
	}
	for (int x = 0; x < CostTable.size(); x++) {
		for (int z = 0; z < CostTable[x].size(); z++) {
			CostTable[x][z] = m_Data[x][z].Cost;
		}
	}
	isHitChip = false;
}

void Map::Update()
{
	HitCheckMouseRayMapPosition();
	m_DrawGuideCount = 0;

	for (int i = 0; i < TORCH_MAX; i++) {
		m_TorchBloomDatas[i].Rot.x += 1.0f;
		m_TorchBloomDatas[i].Rot.z += 1.0f;
		torchBloom->object[i].rotation = m_TorchBloomDatas[i].Rot;
	}

	/*ImGui::SetNextWindowPos(ImVec2(10, 60), 1);
	ImGui::SetNextWindowSize(ImVec2(300, 100), 1);
	ImGui::Begin("Config");
	ImGui::SliderInt("GenerateEnemyCount", &GenerateEnemyCount, 1, 4);
	if (ImGui::Button("Reset")) {
		m_enemys.AllEnemyClear();
		m_enemys.RandamEnemyGenerate(GenerateEnemyCount);
		Turn = 2;
		m_cards.StartTurn();
	}
	ImGui::End();*/
	for (int i = 0; i < GUIDE_MAX; i++) {
		m_isGuideDraw[i] = false;
		m_GuideDrawPriority[i] = 0;
	}
}

void Map::Draw()
{
	Drawobject3d(m_StageModel);
	Drawobject3d(m_StageModel_saku);
	Drawobject3d(m_StageModel_floor);
	//Drawobject3d(m_StageModel_tree);
	Drawobject3d(torch);
	Drawobject3d(torchBloom);
	Drawobject3d(m_Pillar);

	for (auto &dataX : m_Data) {
		for (auto &dataY : dataX) {
			if (dataY.Cost != 9999) {
				dataY.Draw();
			}
		}
	}

	for (int i = 0; i < GUIDE_MAX; i++) {
		ChipGuide->object[i].isDraw = m_isGuideDraw[i];
	}

	if (NowHitChip.x != -1) {

		DrawGuide(NowHitChip, ColorConvert(0, 255, 0, 255), GuidePriority::NOW_HIT);

		/*XMINT3 pp = m_Data[NowHitChip.x][NowHitChip.y].m_Pos;
		DrawCube(XMFLOAT3(float(pp.x * ChipData::CHIP_SIZE), float(pp.y), float(pp.z * ChipData::CHIP_SIZE)),
			XMFLOAT3(float(pp.x * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE), float(pp.y) + 0.5f, float(pp.z * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE)),
			ColorConvert(0, 255, 0, 150));*/
	}
	if (NowSelectChip.x != -1) {

		DrawGuide(NowSelectChip, ColorConvert(255, 255, 0, 255), GuidePriority::SELECT);

		/*XMINT3 pp = m_Data[NowSelectChip.x][NowSelectChip.y].m_Pos;
		DrawCube(XMFLOAT3(float(pp.x * ChipData::CHIP_SIZE), float(pp.y), float(pp.z * ChipData::CHIP_SIZE)),
			XMFLOAT3(float(pp.x * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE), float(pp.y) + 1.0f, float(pp.z * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE)),
			ColorConvert(255, 255, 0, 150));*/
	}
	Drawobject3d(ChipGuide);
}

void Map::ShadowDraw()
{
	ShadowDepthDrawobject3d(m_StageModel);
	ShadowDepthDrawobject3d(m_StageModel_saku);
	ShadowDepthDrawobject3d(m_StageModel_floor);
	//ShadowDepthDrawobject3d(m_StageModel_tree);
	ShadowDepthDrawobject3d(torch);
	ShadowDepthDrawobject3d(m_Pillar);
}

void Map::DepthDraw()
{
	DepthDrawobject3d(m_StageModel);
	DepthDrawobject3d(m_StageModel_saku);
	DepthDrawobject3d(m_StageModel_floor);
	//DepthDrawobject3d(m_StageModel_tree);
	DepthDrawobject3d(torch);
	DepthDrawobject3d(torchBloom);
	DepthDrawobject3d(m_Pillar);
}

void Map::DOFDepthDraw()
{
	DOFDepthDrawobject3d(m_StageModel);
	DOFDepthDrawobject3d(m_StageModel_saku);
	DOFDepthDrawobject3d(m_StageModel_floor);
	//DOFDepthDrawobject3d(m_StageModel_tree);
	DOFDepthDrawobject3d(torch);
	DOFDepthDrawobject3d(ChipGuide);
	DOFDepthDrawobject3d(m_Pillar);
}

void Map::BloomDraw()
{
	Drawobject3d(torchBloom);
}

void Map::BloomDepthDraw()
{
	DepthDrawobject3d(torchBloom);
}

XMFLOAT3 Map::ChangePos(XMINT2 MapPos)
{
	XMFLOAT3 Pos = XMFLOAT3(m_Data[MapPos.x][MapPos.y].m_Pos.x * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE / 2.0f,
		m_Data[MapPos.x][MapPos.y].m_Pos.y * ChipData::CHIP_SIZE,
		m_Data[MapPos.x][MapPos.y].m_Pos.z * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE / 2.0f);
	return Pos;
}

bool Map::InMap(XMINT2 MapPos)
{
	if (MapPos.x < m_Data.size() && MapPos.x >= 0 && MapPos.y < m_Data[0].size() && MapPos.y >= 0) {
		return true;
	}
	return false;
}

void Map::SetCostTableOnUnitPos(XMINT2 MapPos, int Cost)
{
	CostTable[MapPos.x][MapPos.y] = Cost;
}

void Map::ResetCostTable()
{
	for (int x = 0; x < CostTable.size(); x++) {
		for (int z = 0; z < CostTable[x].size(); z++) {
			CostTable[x][z] = m_Data[x][z].Cost;
		}
	}
}

void Map::HitCheckMouseRayMapPosition()
{
	if (Input::isMouseTrigger(M_LEFT) && isHitChip == true) {
		NowSelectChip = NowHitChip;
	}
	if (Input::GetMouseMove() == Input::MouseMove((LONG)0, (LONG)0, (LONG)0)) {
		return;
	}
	Ray mRay = Input::GetMouseRay();
	XMFLOAT3 normal = XMFLOAT3(0, 1, 0);
	DirectX::XMVECTOR Vnormal = XMLoadFloat3(&normal);

	// 各チップとの判定
	XMINT2 HitChipNum = { -1, -1 };
	float NowDist = 9999.0f;

	for (int x = 0; x < m_Data.size(); x++) {
		for (int z = 0; z < m_Data[x].size(); z++) {
			XMFLOAT3 point[4];
			point[0] = XMFLOAT3(float(m_Data[x][z].m_Pos.x * ChipData::CHIP_SIZE), float(m_Data[x][z].m_Pos.y), float(m_Data[x][z].m_Pos.z * ChipData::CHIP_SIZE));
			point[1] = XMFLOAT3(float(m_Data[x][z].m_Pos.x * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE), float(m_Data[x][z].m_Pos.y), float(m_Data[x][z].m_Pos.z * ChipData::CHIP_SIZE));
			point[2] = XMFLOAT3(float(m_Data[x][z].m_Pos.x * ChipData::CHIP_SIZE), float(m_Data[x][z].m_Pos.y), float(m_Data[x][z].m_Pos.z * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE));
			point[3] = XMFLOAT3(float(m_Data[x][z].m_Pos.x * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE), float(m_Data[x][z].m_Pos.y), float(m_Data[x][z].m_Pos.z * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE));

			float Dist = 9999.9f;
			Triangle t1;
			t1.p0 = XMLoadFloat3(&point[1]);
			t1.p1 = XMLoadFloat3(&point[0]);
			t1.p2 = XMLoadFloat3(&point[2]);
			t1.normal = Vnormal;
			Triangle t2;
			t2.p0 = XMLoadFloat3(&point[3]);
			t2.p1 = XMLoadFloat3(&point[1]);
			t2.p2 = XMLoadFloat3(&point[2]);
			t2.normal = Vnormal;
			if (Collision2::CheckRay2Triangle(mRay, t1, &Dist)) {
				if (NowDist > Dist) {
					HitChipNum = { x, z };
					NowDist = Dist;
				}
			}
			else if (Collision2::CheckRay2Triangle(mRay, t2, &Dist)) {
				if (NowDist > Dist) {
					HitChipNum = { x, z };
					NowDist = Dist;
				}
			}
		}
	}

	if (HitChipNum.x != -1) {
		NowHitChip = HitChipNum;
		isHitChip = true;
	}
	else {
		NowHitChip = HitChipNum;
		isHitChip = false;
	}
}

void Map::DrawGuide(XMINT2 MapPos, XMFLOAT4 Color, GuidePriority Priority, bool isDrawSide)
{
	if (m_Data[MapPos.x][MapPos.y].Cost > 999) {
		return;
	}
	if (!InMap(MapPos)) {
		return;
	}
	if (m_DrawGuideCount > GUIDE_MAX - 1) {
		return;
	}
	XMFLOAT3 DrawPos = ChangePos(MapPos);
	// 既にガイドが描画されている場合に上書きするか
	for (int i = 0; i < m_DrawGuideCount; i++) {
		if (ChipGuide->object[i].position == DrawPos && m_GuideDrawPriority[i] < int(Priority)) {
			ChipGuide->object[i].isDraw = true;
			m_isGuideDraw[i] = true;
			m_GuideDrawPriority[i] = int(Priority);
			ChipGuide->object[i].position = ChangePos(MapPos);
			ChipGuide->object[i].color = Color;
			if (!isDrawSide) {
				ChipGuide->object[i].color.w = -Color.w;
			}
			return;
		}
		else if (ChipGuide->object[i].position == DrawPos && m_GuideDrawPriority[i] < int(Priority)) {
			return;
		}
	}
	ChipGuide->object[m_DrawGuideCount].isDraw = true;
	m_isGuideDraw[m_DrawGuideCount] = true;
	m_GuideDrawPriority[m_DrawGuideCount] = int(Priority);
	ChipGuide->object[m_DrawGuideCount].position = ChangePos(MapPos);
	ChipGuide->object[m_DrawGuideCount].color = Color;
	if (!isDrawSide) {
		ChipGuide->object[m_DrawGuideCount].color.w = -Color.w;
	}
	m_DrawGuideCount++;
}
