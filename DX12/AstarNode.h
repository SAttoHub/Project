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

	int x;		// ��
	int y;		// �s

	Cell operator= (const DirectX::XMINT2 value) {
		return Cell(value.x, value.y);
	}
};

// �m�[�h
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

	Cell Position;		// �m�[�h���W
	std::vector<AstarNode*> AdjacentNodes;	// �אڃm�[�h(��)
	float HeuristicCost;	// �q���[���X�e�B�b�N�R�X�g
	float TotalCost;	// �R�X�g(�q���[���X�e�B�b�N�R�X�g����)
};