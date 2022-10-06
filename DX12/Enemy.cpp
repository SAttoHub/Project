#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
	m_MapPos = { 0,0 };
	m_HP = 1;

	int modelData = LoadModelOBJ("Charactor", "mob");
	model = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);
	model->rotation.y = 90.0f;
	model->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f);

	m_MyTurn = false;
	m_Next = false;
	coolTime = 15;

	Act = AC_Wait;
}

Enemy::~Enemy()
{
	//DirectX3dObject::DeleteObject(model);
	//delete model;
}

void Enemy::StartTurn()
{
	FindRouteToPlayer();
	m_MyTurn = true;
	m_Next = false;
	coolTime = 30;
	Count = 3;
	pMap->ResetCostTable();
	if (m_Route.route_list.size() > 2) {
		Act = AC_Move;
	}
	else {
		Act = AC_Attack;
	}
}

void Enemy::SetMap(Map *Map)
{
	pMap = Map;
}

void Enemy::SetPlayer(Player *player)
{
	pPlayer = player;
}

void Enemy::FindRouteToPlayer()
{
	XMINT2 start = GetMapPos();
	XMINT2 goal = pPlayer->GetMapPos();

	m_Route = Astar::Instance()->DoAstar(Cell(start.x, start.y), Cell(goal.x, goal.y), pMap->CostTable);
}

void Enemy::Update()
{
	/*if (Input::isKeyTrigger(DIK_R)) {
		FindRouteToPlayer();
	}
	if (Input::isKeyTrigger(DIK_RETURN)) {
		std::vector<Cell> result(m_Route.route_list.begin(), m_Route.route_list.end());
		Cell p = result[0];
		SetMapPos(XMINT2(p.x, p.y));
		m_Route.route_list.pop_front();
	}*/
	if (m_MyTurn) {
		if (Act == AC_Move) {
			coolTime--;
			if (coolTime == 0 && Count > 0) {
				Count--;
				coolTime = 15;
				std::vector<Cell> result(m_Route.route_list.begin(), m_Route.route_list.end());
				Cell p = result[0];
				SetMapPos(XMINT2(p.x, p.y));
				m_Route.route_list.pop_front();
				if (m_Route.route_list.size() <= 1) {
					if (Count != 0) {
						m_MyTurn = false;
						m_Next = true;
						Act = AC_Attack;
					}
					else {
						m_MyTurn = false;
						m_Next = true;
						Act = AC_Wait;
					}
				}
			}
			else if (Count == 0) {
				m_MyTurn = false;
				m_Next = true;
				Act = AC_Wait;
			}
		}
		if (Act == AC_Attack) {
			coolTime--;
			if (coolTime == 0) {
				coolTime = 15;
				pPlayer->Damage(1);
				m_MyTurn = false;
				m_Next = true;
				Act = AC_Wait;
			}
		}
	}

	XMFLOAT3 aaa = pMap->ChangePos(m_MapPos);

	model->position = pMap->ChangePos(m_MapPos);
}

void Enemy::Draw()
{
	if (m_MyTurn) {
		model->material.texNumber = TexManager::GetColor(XMFLOAT4(255, 0, 0, 255));
	}
	else {
		model->material.texNumber = TexManager::GetColor(XMFLOAT4(125, 0, 0, 255));
	}
	Drawobject3d(model);
}

void Enemy::Death()
{
	DirectX3dObject::DeleteObject(model);
}
