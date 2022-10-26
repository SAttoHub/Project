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
	model->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.3f;
	model->isBillboard = true;

	m_MyTurn = false;
	m_Next = false;
	coolTime = 15;

	Act = AC_Wait;

	Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn1.png");
	Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn2.png");
	Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn3.png");
	Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn4.png");
	Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn5.png");
	Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn6.png");
	Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn7.png");
	Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn8.png");

	model->color.w = -1.0f;
	model->alwaysUpdate = true;
}

Enemy::~Enemy()
{
	//DirectX3dObject::DeleteObject(model);
	//delete model;
	//delete pMap;
	//delete pPlayer;
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
	SetDir();
	model->material.texNumber = Image[(int)m_Dir];
	if (m_MyTurn) {
		model->color.x = 1.0f;
		model->color.y = 1.0f;
		model->color.z = 1.0f;
	}
	else {
		model->color.x = 0.5f;
		model->color.y = 0.5f;
		model->color.z = 0.5f;
	}
}

void Enemy::Draw()
{
	if (m_MyTurn) {
		model->color.x = 1.0f;
		model->color.y = 1.0f;
		model->color.z = 1.0f;
	}
	else {
		model->color.x = 0.5f;
		model->color.y = 0.5f;
		model->color.z = 0.5f;
	}
	Drawobject3d(model);
}

void Enemy::Death()
{
	DirectX3dObject::DeleteObject(model);
}
