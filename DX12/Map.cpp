#include "Map.h"

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

	int modelData = LoadModelOBJ("Ground_Data", "stage");
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
	m_StageModel_tree->position.x = 32.0f * 10.0f / 4.0f;

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
	m_Data.resize(10);
	for (auto &dataX : m_Data) {
		dataX.resize(10);
	}
	for (int x = 0; x < m_Data.size(); x++) {
		for (int z = 0; z < m_Data[x].size(); z++) {
			m_Data[x][z].m_Pos = XMINT3(x, 0, z);
		}
	}
	CostTable.resize(10);
	for (auto &dataX : CostTable) {
		dataX.resize(10);
	}
	for (int x = 0; x < CostTable.size(); x++) {
		for (int z = 0; z < CostTable[x].size(); z++) {
			CostTable[x][z] = 1;
		}
	}
}

void Map::Update()
{
}

void Map::Draw()
{
	Drawobject3d(m_StageModel);
	Drawobject3d(m_StageModel_saku);
	Drawobject3d(m_StageModel_tree);

	for (auto &dataX : m_Data) {
		for (auto &dataY : dataX) {
			dataY.Draw();
		}
	}
}

void Map::ShadowDraw()
{
	ShadowDepthDrawobject3d(m_StageModel);
	ShadowDepthDrawobject3d(m_StageModel_saku);
	//ShadowDepthDrawobject3d(m_StageModel_tree);
}

void Map::DepthDraw()
{
	DepthDrawobject3d(m_StageModel);
	DepthDrawobject3d(m_StageModel_saku);
	DepthDrawobject3d(m_StageModel_tree);
}

void Map::DOFDepthDraw()
{
	DOFDepthDrawobject3d(m_StageModel);
	DOFDepthDrawobject3d(m_StageModel_saku);
	DOFDepthDrawobject3d(m_StageModel_tree);
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
