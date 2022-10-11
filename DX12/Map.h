#pragma once
#include <vector>
#include "DirectXMathExpansion.h"
#include "PrimitiveFunc.h"
#include "ChipData.h"
#include "3DObject.h"

class Map {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMINT3 = DirectX::XMINT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	
	Object3d *m_StageModel;
	Object3d *m_StageModel_saku;
	Object3d *m_StageModel_tree;

public:
	Map();
	~Map();

	std::vector<std::vector<ChipData>> m_Data;
	std::vector<std::vector<int>> CostTable;
	XMINT2 NowHitChip;
	bool isHitChip;

	ChipData* Get(XMINT2 Pos);
	void Initialize();

	void Update();
	void Draw();

	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();

	XMFLOAT3 ChangePos(XMINT2 MapPos);
	bool InMap(XMINT2 MapPos);

	void SetCostTableOnUnitPos(XMINT2 MapPos, int Cost);
	void ResetCostTable();

	void HitCheckMouseRayMapPosition();
};

