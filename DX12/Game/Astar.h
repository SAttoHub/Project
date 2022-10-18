#pragma once
#include <stdio.h>
#include <memory.h>
#include <list>
#include <queue>
#include <vector>
#include <math.h>

#include "..\Singleton.h"
#include "..\Engine\CommonMathIncludes.h"
#include "AstarNode.h"

using namespace std;


// �폜����
enum EraseResult
{
	NotFound,		// ������
	Erased,			// �폜
	CouldntErased	// �폜�ł��Ȃ�
};

struct AstarResults {
	std::list<Cell> route_list;
	int Cost;
	AstarResults()
	{
		Cost = 999;
	}
	AstarResults(std::list<Cell> route_list, int Cost) :
		route_list(route_list), Cost(Cost){
	}
	~AstarResults(){}
};

class Astar : public Singleton<Astar> {
	friend class Singleton<Astar>;
private:
	using XMINT2 = DirectX::XMINT2;

private:
	const int Infinity = 100000;
	// �v�Z�p�f�[�^
	vector<std::vector<AstarNode>> Map;
	// �v�Z�p�f�[�^���Z�b�g
	void DataReset();

public:
	// �m�[�h�̍쐬
	void CreateMap(const int MapWidth, const int MapHeight, const vector<std::vector<int>> &CostTable);
	// �Z���͈̓`�F�b�N�֐�
	bool IsCellWithinTheRange(int x, int y, int MapWidth, int MapHeight);
	// �R�X�g������
	void InitCost(int heuristic_cost, int total_cost, int MapWidth, int MapHeight);
	// �q���[���X�e�B�b�N�R�X�g�v�Z(�m�[�h�ƃS�[���܂ł̋�����Ԃ��Ă���)
	float CalculateHeuristic(const AstarNode *node, const AstarNode *Goal);
	// �Z����r
	bool IsEqualCell(const Cell &a, const Cell &b);
	EraseResult EraseNode(std::list<AstarNode *> &list, AstarNode *new_node, float new_cost);
	// �I�[�v�����X�g�ɒǉ�
	bool AddAdjacentNode(std::list<AstarNode *> &open_list, std::list<AstarNode *> &close_list, AstarNode *adjacent_node, float cost);

public:
	// A*���s
	AstarResults DoAstar(Cell start, Cell goal, const vector<std::vector<int>> &CostTable);
	// �S�Ẵ��[�g�ɑ΂���A*���s
	vector<std::vector<AstarResults>> DoAstarForAll(XMINT2 start, const vector<std::vector<int>> &CostTable);
	// �R�X�g��xy���̍��v�̍���菬�����`�b�v�ɑ΂��Ă̑S�Ẵ��[�g�ɑ΂���A*���s
	vector<std::vector<AstarResults>> DoAstarForAllSaving(XMINT2 start, const vector<std::vector<int>> &CostTable, int Cost);
};

