#include "Astar.h"

void Astar::DataReset()
{
	for (int i = 0; i < Map.size(); i++) {
		for (int j = 0; j < Map[i].size(); j++) {
			Map[i][j].AdjacentNodes.clear();
			Map[i][j].AdjacentNodes.shrink_to_fit();
		}
		Map[i].clear();
		Map[i].shrink_to_fit();
	}
	Map.clear();
	Map.shrink_to_fit();
}

bool Astar::IsCellWithinTheRange(int x, int y, int MapWidth, int MapHeight)
{
	if (x >= 0 &&
		x < MapWidth &&
		y >= 0 &&
		y < MapHeight)
	{
		return true;
	}

	return false;
}

bool Less(AstarNode *a, AstarNode *b)
{
	if (a->TotalCost < b->TotalCost)
	{
		return true;
	}

	return false;
}

void Astar::InitCost(int heuristic_cost, int total_cost, int MapWidth, int MapHeight)
{
	for (int x = 0; x < MapWidth; x++)
	{
		for (int y = 0; y < MapHeight; y++)
		{
			Map[x][y].HeuristicCost = heuristic_cost;
			Map[x][y].TotalCost = total_cost;
		}
	}
	/*for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			Map[y][x].HeuristicCost = heuristic_cost;
			Map[y][x].TotalCost = total_cost;
		}
	}*/
}

float Astar::CalculateHeuristic(const AstarNode *node, const AstarNode *Goal)
{
	float x = fabsf(Goal->Position.x - node->Position.x);
	float y = fabsf(Goal->Position.y - node->Position.y);

	return sqrtf(x * x + y * y);
}

bool Astar::IsEqualCell(const Cell &a, const Cell &b)
{
	if (a.x == b.x &&
		a.y == b.y)
	{
		return true;
	}

	return false;
}

EraseResult Astar::EraseNode(std::list<AstarNode *> &list, AstarNode *new_node, float new_cost)
{
	// クローズリストチェック
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		// ノードと同じセルがあるか調べる
		if (IsEqualCell(new_node->Position, (*itr)->Position) == true)
		{
			// コストの比較
			if (new_cost < (*itr)->TotalCost)
			{
				list.erase(itr);
				return EraseResult::Erased;
			}
			else
			{
				return EraseResult::CouldntErased;
			}
		}
	}

	return EraseResult::NotFound;
}

bool Astar::AddAdjacentNode(std::list<AstarNode *> &open_list, std::list<AstarNode *> &close_list, AstarNode *adjacent_node, float cost)
{
	bool can_update = true;

	std::list<AstarNode *> node_list[] =
	{
		close_list,
		open_list
	};

	for (std::list<AstarNode *> &list : node_list)
	{
		// リストに同じノードがあってリストの方のコストが高いなら削除
		if (EraseNode(list, adjacent_node, cost) == EraseResult::CouldntErased)
		{
			can_update = false;
		}
	}

	if (can_update == true)
	{
		open_list.push_back(adjacent_node);
		return true;
	}

	return false;
}

void Astar::CreateMap(const int MapWidth, const int MapHeight, const vector<std::vector<int>> &CostTable)
{
	for (int x = 0; x < MapWidth; x++)
	{
		for (int y = 0; y < MapHeight; y++)
		{
			Map[x][y].Position.x = x;
			Map[x][y].Position.y = y;

			XMINT2 adjacent_cells[] =
			{
				XMINT2(x, y - 1),
				XMINT2(x - 1, y),
				XMINT2(x + 1, y),
				XMINT2(x, y + 1),
			};

			// 隣接ノードの追加
			for (const XMINT2 &cell : adjacent_cells)
			{
				if (IsCellWithinTheRange(cell.x, cell.y, MapWidth, MapHeight) == true &&
					CostTable[cell.x][cell.y] == 1)
				{
					Map[x][y].AdjacentNodes.push_back(&Map[cell.x][cell.y]);
				}
			}
		}
	}

	//for (int y = 0; y < MapHeight; y++)
	//{
	//	for (int x = 0; x < MapWidth; x++)
	//	{
	//		Map[y][x].Position.x = x;
	//		Map[y][x].Position.y = y;

	//		Cell adjacent_cells[] =
	//		{
	//			Cell(x, y - 1),
	//			Cell(x - 1, y),
	//			Cell(x + 1, y),
	//			Cell(x, y + 1),
	//		};

	//		// 隣接ノードの追加
	//		for (const Cell &cell : adjacent_cells)
	//		{
	//			if (IsCellWithinTheRange(cell.x, cell.y, MapWidth, MapHeight) == true &&
	//				CostTable[cell.y][cell.x] == 1)
	//			{
	//				Map[y][x].AdjacentNodes.push_back(&Map[cell.y][cell.X]);
	//			}
	//		}
	//	}
	//}
}

AstarResults Astar::DoAstar(Cell start, Cell goal, const vector<std::vector<int>> &CostTable)
{
	int Width = CostTable.size(), Height = CostTable[0].size();

	DataReset();
	Map.resize(Width);
	for (int x = 0; x < Width; ++x) {
		Map[x].resize(Height);
	}
	CreateMap(Width, Height, CostTable);

	std::list<AstarNode *> open_list;
	std::list<AstarNode *> close_list;

	//const Node* start_node = &Map[start.x][start.y];
	const AstarNode *goal_node = &Map[goal.x][goal.y];

	// 更新したノード位置保存用
	vector<std::vector<Cell>> last_update_cells;
	last_update_cells.resize(Width);
	for (int x = 0; x < Width; ++x) {
		last_update_cells[x].resize(Height);
	}

	// グラフの初期化
	InitCost(Infinity, 0, Width, Height);
	// スタートノードの指定
	open_list.push_back(&Map[start.x][start.y]);
	// 経路探索回数
	int count = 0;

	// オープンリストがなくなるまで回す
	while (open_list.empty() == false)
	{
		count++;

		AstarNode *search_node = (*open_list.begin());
		// 探索リストから除外
		open_list.erase(open_list.begin());

		// ゴールに到達したら終わり
		if (IsEqualCell(search_node->Position, goal) == true)
		{
			// クローズリストに最後のノードを追加する
			close_list.push_back(search_node);
			break;
		}

		for (AstarNode *adjacent_node : search_node->AdjacentNodes)
		{
			// 計算を行っていなかった場合だけ計算
			if (adjacent_node->HeuristicCost == Infinity)
			{
				// ヒューリスティクスコスト計算
				adjacent_node->HeuristicCost = CalculateHeuristic(adjacent_node, goal_node);
			}

			// ノード間コスト
			float edge_cost = CostTable[adjacent_node->Position.x][adjacent_node->Position.y];
			// 取得ノードのトータルコスト
			float node_cost = search_node->TotalCost;
			/*
				トータルコスト算出
					ノード間コスト + ヒューリスティックコスト + 取得ノードのトータルコスト
			*/
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			// ノード追加
			if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost) == true)
			{
				// トータルコストを更新
				adjacent_node->TotalCost = total_cost;

				if (adjacent_node->Position.y == 0 && adjacent_node->Position.x == 2)
				{
					int xx = 0;
					xx = 100;
				}

				// 経路を更新したセルを保存
				last_update_cells[adjacent_node->Position.x][adjacent_node->Position.y] = search_node->Position;
			}
		}

		bool is_add_close = true;

		// クローズリストチェック
		for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
		{
			// ノードと同じセルがあるか調べる
			if (IsEqualCell(search_node->Position, (*itr)->Position) == true)
			{
				is_add_close = false;
				break;
			}
		}

		// 同じノードが無かったので追加
		if (is_add_close == true)
		{
			// このノードの探索終了
			close_list.push_back(search_node);
		}

		// 昇順ソート
		open_list.sort(Less);
	}

	// 経路復元
	std::list<Cell> route_list;

	// 必要コスト
	int NeedCost = 0;

	// ゴールセルから復元する
	route_list.push_back(goal);
	while (route_list.empty() == false)
	{
		Cell route = route_list.front();

		// スタートセルなら終了
		if (IsEqualCell(route, start) == true)
		{
			NeedCost = 0;
			// 復元した経路のコストを計算
			for (Cell &cell : route_list)
			{
				NeedCost += CostTable[cell.x][cell.y];
			}
			break;
		}
		else
		{
			if (IsCellWithinTheRange(route.x, route.y, Width, Height) == true)
			{
				// 追加
				route_list.push_front(last_update_cells[route.x][route.y]);
			}
			else
			{
				// 経路は見つからなかった
				break;
			}
		}
	}

	return AstarResults(route_list, NeedCost);
}

vector<std::vector<AstarResults>> Astar::DoAstarForAll(XMINT2 start, const vector<std::vector<int>> &CostTable)
{
	vector<std::vector<AstarResults>> Result;
	Result.resize(CostTable.size());
	for (int x = 0; x < CostTable.size(); x++) {
		Result[x].resize(CostTable[x].size());
	}

	for (int x = 0; x < CostTable.size(); x++) {
		for (int y = 0; y < CostTable[x].size(); y++) {
			if (start.x != x || start.y != y) {
				Result[x][y] = DoAstar(Cell(start.x, start.y), Cell(x, y), CostTable);
			}
			else {
				Result[x][y].Cost = 0;
			}
		}
	}
	return Result;
}

vector<std::vector<AstarResults>> Astar::DoAstarForAllSaving(XMINT2 start, const vector<std::vector<int>> &CostTable, int Cost)
{
	vector<std::vector<AstarResults>> Result;
	Result.resize(CostTable.size());
	for (int x = 0; x < CostTable.size(); x++) {
		Result[x].resize(CostTable[x].size());
	}

	for (int x = 0; x < CostTable.size(); x++) {
		for (int y = 0; y < CostTable[x].size(); y++) {
			int sa = abs(start.x - x) + abs(start.y - y);
			if (Cost >= sa && (start.x != x || start.y != y)) {
				Result[x][y] = DoAstar(Cell(start.x, start.y), Cell(x, y), CostTable);
			}
			else {
				Result[x][y].Cost = 0;
			}
		}
	}
	return Result;
}
