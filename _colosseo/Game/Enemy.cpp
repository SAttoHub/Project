#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
	m_MapPos = { 0,0 };
	m_HP = 1;
	m_MaxHP = 1;

	int modelData = LoadModelOBJ("Charactor", "mob");
	m_pModel = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(-1000, -1000, -1000), FBXSHADER);
	m_pModel->rotation.y = 90.0f;
	m_pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.3f;
	m_pModel->isBillboard = true;
	m_pModel->UseShadow = true;
	m_pModel->UseDOF = true;

	m_MyTurn = false;
	m_Next = false;
	m_CoolTime = 15;

	m_Act = AC_Wait;

	m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn8.png");

	m_pModel->color.w = -1.0f;
	m_pModel->alwaysUpdate = true;
}

Enemy::~Enemy()
{
	//DirectX3dObject::DeleteObject(model);
	//delete model;
	//delete pMap;
	//delete pPlayer;
}

void Enemy::Initialize()
{
	m_MapPos = { 0,0 };
	m_HP = 5;
	m_MaxHP = 5;

	int modelData = LoadModelOBJ("Charactor", "mob");
	m_pModel = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(-1000, -1000, -1000), FBXSHADER);
	m_pModel->rotation.y = 90.0f;
	m_pModel->scale = XMFLOAT3(16 / 4.0f, 16 / 4.0f, 16 / 4.0f) * 1.3f;
	m_pModel->isBillboard = true;
	m_pModel->UseShadow = true;
	m_pModel->UseDOF = true;

	m_MyTurn = false;
	m_Next = false;
	m_CoolTime = 15;

	m_Act = AC_Wait;

	m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn8.png");

	m_pModel->color.w = -1.0f;
	m_pModel->alwaysUpdate = true;
}

void Enemy::Finalize()
{
}

void Enemy::StartTurn()
{
	FindRouteToPlayer();
	m_MyTurn = true;
	m_Next = false;
	m_CoolTime = 30;
	m_Count = 3;
	m_pMap->ResetCostTable();
	if (m_Route.route_list.size() > 2) {
		m_Act = AC_Move;
	}
	else {
		m_Act = AC_Attack;
	}
	CameraTargetOnMe(true);
}

void Enemy::SetMap(Map *Map)
{
	m_pMap = Map;
}

void Enemy::SetPlayer(Player *player)
{
	pPlayer = player;
}

void Enemy::FindRouteToPlayer()
{
	XMINT2 start = GetMapPos();
	XMINT2 goal = pPlayer->GetMapPos();

	m_Route = Astar::Instance()->DoAstar(Cell(start.x, start.y), Cell(goal.x, goal.y), m_pMap->CostTable);
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

	WaitUpdate();
	if (!isWait()) {
		if (m_MyTurn) {
			if (m_Act == AC_Move) {
				m_CoolTime--;
				if (m_CoolTime == 0 && m_Count > 0) {
					m_Count--;
					m_CoolTime = 15;
					std::vector<Cell> result(m_Route.route_list.begin(), m_Route.route_list.end());
					Cell p = result[0];
					SetMapPos(XMINT2(p.x, p.y));
					CameraTargetOnMe(true, WAIT_TIMER_VALUE_MOVE);
					m_Route.route_list.pop_front();
					if (m_Route.route_list.size() <= 1) {
						if (m_Count != 0) {
							m_MyTurn = false;
							m_Next = true;
							m_Act = AC_Attack;
						}
						else {
							m_MyTurn = false;
							m_Next = true;
							m_Act = AC_Wait;
						}
					}
				}
				else if (m_Count == 0) {
					m_MyTurn = false;
					m_Next = true;
					m_Act = AC_Wait;
				}
			}
			if (m_Act == AC_Attack) {
				m_CoolTime--;
				if (m_CoolTime == 0) {
					m_CoolTime = 15;
					pPlayer->Damage(1);
					pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
					m_MyTurn = false;
					m_Next = true;
					m_Act = AC_Wait;
				}
			}
		}
	}

	ReactionUpdate();

	m_pModel->position = GetWorldPos() + m_ReactionOffset;
	SetDirFromCamera();
	m_pModel->material.texNumber = m_Image[(int)m_DirFromCamera];
	if (m_MyTurn) {
		m_pModel->color.x = 1.0f;
		m_pModel->color.y = 1.0f;
		m_pModel->color.z = 1.0f;
	}
	else {
		m_pModel->color.x = 0.5f;
		m_pModel->color.y = 0.5f;
		m_pModel->color.z = 0.5f;
	}
}

void Enemy::Draw()
{
	if (m_MyTurn) {
		m_pModel->color.x = 1.0f;
		m_pModel->color.y = 1.0f;
		m_pModel->color.z = 1.0f;
		m_pMap->DrawGuide(GetMapPos(), ColorConvert(228, 86, 83, 255), GuidePriority::NOW_ACT_UNIT);
	}
	else {
		m_pModel->color.x = 0.5f;
		m_pModel->color.y = 0.5f;
		m_pModel->color.z = 0.5f;
	}
	Drawobject3d(m_pModel);
}

void Enemy::Death()
{
	DirectX3dObject::DeleteObject(m_pModel);
}

std::vector<AllResult> Enemy::AttackAct(int Index, XMINT2 Pos) {
	// UŒ‚•ûŒü‚ð”»’è
	XMINT2 Hoge = m_MapPos - Pos;

	Enemy_Attack_Data AtkData = m_Attack_Datas[Index];

	KnockBack KnB(AtkData.m_KnockBackPower,
		Hoge.y > 0 ? true : false,
		Hoge.y < 0 ? true : false,
		Hoge.x < 0 ? true : false,
		Hoge.x > 0 ? true : false);
	return ActionManager::Action(Pos, AtkData.m_Damage, KnB, AtkData.m_Abn, AtkData.m_AbnTurn);
}