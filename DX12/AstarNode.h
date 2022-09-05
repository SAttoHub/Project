#pragma once
#include <vector>
#include "DirectXMathExpansion.h"

struct Cell
{
	Cell() :
		x(-1),
		y(-1)
	{
	}

	Cell(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	int x;		// 列
	int y;		// 行

	Cell operator= (const DirectX::XMINT2 value) {
		return Cell(value.x, value.y);
	}
};

// ノード
struct AstarNode
{
private:
	using XMINT2 = DirectX::XMINT2;

public:
	AstarNode() :
		AstarNode(0, 0)
	{}

	AstarNode(int x, int y) :
		Position(x, y),
		HeuristicCost(100000),
		TotalCost(0)
	{
		AdjacentNodes.clear();
	}

	Cell Position;		// ノード座標
	std::vector<AstarNode*> AdjacentNodes;	// 隣接ノード(辺)
	float HeuristicCost;	// ヒューリスティックコスト
	float TotalCost;	// コスト(ヒューリスティックコスト込み)
};