#pragma once
#include <vector>
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "ChipData.h"
#include "..\Engine\Objects\3DObject.h"

#include "GameUIsManager.h"

enum class GuidePriority {
	MOVE_OR_ATTACK,
	NOW_ACT_UNIT,
	NOW_HIT,
	SELECT,
	SELECT_ENEMY_PRE_CARD_USE
};

//-------------------------------------------------------------------------------------------------------------
// 戦闘シーンマップ
//-------------------------------------------------------------------------------------------------------------
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
	std::vector<std::vector<ChipData>> m_Data;
	Object3d *m_StageModel = nullptr;
	Object3d *m_StageModel_saku = nullptr;

	Object3d* m_StageModel_floor = nullptr;

	InstanceObjectsData* m_Pillar = nullptr;
	struct pillarData {
		XMFLOAT3 Pos;
		XMFLOAT3 Rot;
		XMFLOAT3 Scale;
	};
	static const int PILLAR_MAX = 4;
	pillarData m_PillarDatas[PILLAR_MAX];

	InstanceObjectsData* torch = nullptr;
	InstanceObjectsData* torchBloom = nullptr;
	struct torchData {
		XMFLOAT3 Pos;
		XMFLOAT3 Rot;
		XMFLOAT3 Scale;
	};
	static const int TORCH_MAX = 8;
	torchData m_TorchDatas[TORCH_MAX];
	torchData m_TorchBloomDatas[TORCH_MAX];

	static const int GUIDE_MAX = 400;
	InstanceObjectsData* ChipGuide = nullptr;
	int m_DrawGuideCount = 0;
	bool m_isGuideDraw[GUIDE_MAX] = { false };
	int m_GuideDrawPriority[GUIDE_MAX] = { 0 };
public:
	Map();
	~Map();

	const int IMMOVABLE_CHIP_COST = 9999;
	
	std::vector<std::vector<int>> CostTable;
	XMINT2 NowHitChip = XMINT2();
	XMINT2 NowSelectChip = XMINT2(-1, -1);
	bool isHitChip = false;

	ChipData* Get(XMINT2 Pos);
	void Initialize();

	void Update();
	void Draw();

	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();
	void BloomDraw();
	void BloomDepthDraw();

	XMFLOAT3 ChangePos(XMINT2 MapPos);
	bool InMap(XMINT2 MapPos);
	bool IsImmovableChip(XMINT2 MapPos);

	void SetCostTableOnUnitPos(XMINT2 MapPos, int Cost);
	void ResetCostTable();

	void HitCheckMouseRayMapPosition();

	static const int m_MAP_SIZE_X = 14;
	static const int m_MAP_SIZE_Z = 14;
	XMFLOAT3 Center;

	void DrawGuide(XMINT2 MapPos, XMFLOAT4 Color, GuidePriority Priority, bool isDrawSide = true);
};

