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


// 削除結果
enum EraseResult
{
	NotFound,		// 未発見
	Erased,			// 削除
	CouldntErased	// 削除できない
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
	// 計算用データ
	vector<std::vector<AstarNode>> Map;
	// 計算用データリセット
	void DataReset();

public:
	// ノードの作成
	void CreateMap(const int MapWidth, const int MapHeight, const vector<std::vector<int>> &CostTable);
	// セル範囲チェック関数
	bool IsCellWithinTheRange(int x, int y, int MapWidth, int MapHeight);
	// コスト初期化
	void InitCost(int heuristic_cost, int total_cost, int MapWidth, int MapHeight);
	// ヒューリスティックコスト計算(ノードとゴールまでの距離を返している)
	float CalculateHeuristic(const AstarNode *node, const AstarNode *Goal);
	// セル比較
	bool IsEqualCell(const Cell &a, const Cell &b);
	EraseResult EraseNode(std::list<AstarNode *> &list, AstarNode *new_node, float new_cost);
	// オープンリストに追加
	bool AddAdjacentNode(std::list<AstarNode *> &open_list, std::list<AstarNode *> &close_list, AstarNode *adjacent_node, float cost);

public:
	// A*実行
	AstarResults DoAstar(Cell start, Cell goal, const vector<std::vector<int>> &CostTable);
	// 全てのルートに対してA*実行
	vector<std::vector<AstarResults>> DoAstarForAll(XMINT2 start, const vector<std::vector<int>> &CostTable);
	// コストがxy軸の合計の差より小さいチップに対しての全てのルートに対してA*実行
	vector<std::vector<AstarResults>> DoAstarForAllSaving(XMINT2 start, const vector<std::vector<int>> &CostTable, int Cost);
};

