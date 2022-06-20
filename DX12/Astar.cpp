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
	// �N���[�Y���X�g�`�F�b�N
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		// �m�[�h�Ɠ����Z�������邩���ׂ�
		if (IsEqualCell(new_node->Position, (*itr)->Position) == true)
		{
			// �R�X�g�̔�r
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
		// ���X�g�ɓ����m�[�h�������ă��X�g�̕��̃R�X�g�������Ȃ�폜
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

			// �אڃm�[�h�̒ǉ�
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

	//		// �אڃm�[�h�̒ǉ�
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

	// �X�V�����m�[�h�ʒu�ۑ��p
	vector<std::vector<Cell>> last_update_cells;
	last_update_cells.resize(Width);
	for (int x = 0; x < Width; ++x) {
		last_update_cells[x].resize(Height);
	}

	// �O���t�̏�����
	InitCost(Infinity, 0, Width, Height);
	// �X�^�[�g�m�[�h�̎w��
	open_list.push_back(&Map[start.x][start.y]);
	// �o�H�T����
	int count = 0;

	// �I�[�v�����X�g���Ȃ��Ȃ�܂ŉ�
	while (open_list.empty() == false)
	{
		count++;

		AstarNode *search_node = (*open_list.begin());
		// �T�����X�g���珜�O
		open_list.erase(open_list.begin());

		// �S�[���ɓ��B������I���
		if (IsEqualCell(search_node->Position, goal) == true)
		{
			// �N���[�Y���X�g�ɍŌ�̃m�[�h��ǉ�����
			close_list.push_back(search_node);
			break;
		}

		for (AstarNode *adjacent_node : search_node->AdjacentNodes)
		{
			// �v�Z���s���Ă��Ȃ������ꍇ�����v�Z
			if (adjacent_node->HeuristicCost == Infinity)
			{
				// �q���[���X�e�B�N�X�R�X�g�v�Z
				adjacent_node->HeuristicCost = CalculateHeuristic(adjacent_node, goal_node);
			}

			// �m�[�h�ԃR�X�g
			float edge_cost = CostTable[adjacent_node->Position.x][adjacent_node->Position.y];
			// �擾�m�[�h�̃g�[�^���R�X�g
			float node_cost = search_node->TotalCost;
			/*
				�g�[�^���R�X�g�Z�o
					�m�[�h�ԃR�X�g + �q���[���X�e�B�b�N�R�X�g + �擾�m�[�h�̃g�[�^���R�X�g
			*/
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			// �m�[�h�ǉ�
			if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost) == true)
			{
				// �g�[�^���R�X�g���X�V
				adjacent_node->TotalCost = total_cost;

				if (adjacent_node->Position.y == 0 && adjacent_node->Position.x == 2)
				{
					int xx = 0;
					xx = 100;
				}

				// �o�H���X�V�����Z����ۑ�
				last_update_cells[adjacent_node->Position.x][adjacent_node->Position.y] = search_node->Position;
			}
		}

		bool is_add_close = true;

		// �N���[�Y���X�g�`�F�b�N
		for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
		{
			// �m�[�h�Ɠ����Z�������邩���ׂ�
			if (IsEqualCell(search_node->Position, (*itr)->Position) == true)
			{
				is_add_close = false;
				break;
			}
		}

		// �����m�[�h�����������̂Œǉ�
		if (is_add_close == true)
		{
			// ���̃m�[�h�̒T���I��
			close_list.push_back(search_node);
		}

		// �����\�[�g
		open_list.sort(Less);
	}

	// �o�H����
	std::list<Cell> route_list;

	// �K�v�R�X�g
	int NeedCost = 0;

	// �S�[���Z�����畜������
	route_list.push_back(goal);
	while (route_list.empty() == false)
	{
		Cell route = route_list.front();

		// �X�^�[�g�Z���Ȃ�I��
		if (IsEqualCell(route, start) == true)
		{
			NeedCost = 0;
			// ���������o�H�̃R�X�g���v�Z
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
				// �ǉ�
				route_list.push_front(last_update_cells[route.x][route.y]);
			}
			else
			{
				// �o�H�͌�����Ȃ�����
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
