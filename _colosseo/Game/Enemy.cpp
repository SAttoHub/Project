#include "Enemy.h"
#include "Player.h"

#include "ActionEffectsMgr.h"

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

	//m_pModel->color.w = -1.0f;
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

	/*m_Image[0] = TexManager::LoadTexture("Resource/image/Chara/CharaEn1.png");
	m_Image[1] = TexManager::LoadTexture("Resource/image/Chara/CharaEn2.png");
	m_Image[2] = TexManager::LoadTexture("Resource/image/Chara/CharaEn3.png");
	m_Image[3] = TexManager::LoadTexture("Resource/image/Chara/CharaEn4.png");
	m_Image[4] = TexManager::LoadTexture("Resource/image/Chara/CharaEn5.png");
	m_Image[5] = TexManager::LoadTexture("Resource/image/Chara/CharaEn6.png");
	m_Image[6] = TexManager::LoadTexture("Resource/image/Chara/CharaEn7.png");
	m_Image[7] = TexManager::LoadTexture("Resource/image/Chara/CharaEn8.png");*/

	//m_pModel->color.w = -1.0f;
	m_pModel->alwaysUpdate = true;
}

void Enemy::Finalize()
{
}

void Enemy::StartTurn()
{
	/*FindRouteToPlayer();
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
	CameraTargetOnMe(true);*/

	StartTurnWhitchAct();
	m_MyTurn = true;
	m_Next = false;
	TurnEndFlag = false;
	m_CoolTime = 30;
	m_MoveCost = m_StartUp.m_MoveCost;
	m_pMap->ResetCostTable();
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

	// 演出後に攻撃する
	if (m_isNowAttack/* && !isWait()*/) {
		AttackEffectUpdate();
		/*m_isNowAttack = false;
		pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
		pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
		TurnEnd();*/
	}

	if (!isWait() && m_MyTurn) {
		if (TurnEndFlag == false) {
			Act();
		}
		else {
			TurnEndTimer > 0 ? TurnEndTimer-- : 0;
			if (TurnEndTimer <= 0) {
				m_MyTurn = false;
				m_Next = true;
				m_Act = AC_Wait;
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
	// 攻撃方向を判定
	XMINT2 Hoge = m_MapPos - Pos;

	Enemy_Attack_Data AtkData = m_Attack_Datas[Index];

	KnockBack KnB(AtkData.m_KnockBackPower,
		Hoge.y > 0 ? true : false,
		Hoge.y < 0 ? true : false,
		Hoge.x < 0 ? true : false,
		Hoge.x > 0 ? true : false);
	return ActionManager::Action(Pos, AtkData.m_Damage, KnB, AtkData.m_Abn, AtkData.m_AbnTurn, ActionManager::AUT_EN);
}

bool Enemy::AttackHitsTheTarget(XMINT2 MapPos, const Enemy_Attack_Data &_Attack_Data)
{
	// 対象の位置
	XMINT2 PlayerPos = pPlayer->GetMapPos();

	// 直線的な攻撃の場合、直線状にターゲットが居ないと当たらない
	if (_Attack_Data.m_IsLinear == true &&
		(PlayerPos.x != MapPos.x && PlayerPos.y != MapPos.y)) {
		return false;
	}

	// 誤差修正用オフセット
	double offset = 0.1;
	// 攻撃対象と自分の距離を求める ( 誤差対策にオフセットを引く )
	double dist = sqrt(pow(MapPos.x - PlayerPos.x, 2) + pow(MapPos.y - PlayerPos.y, 2)) - offset;

	// 攻撃可能範囲内に対象が居るなら攻撃が対象に当たる
	if (double(_Attack_Data.m_Range) >= dist) {
		return true;
	}

	// 攻撃が対象に当たらない
	return false;
}

int Enemy::CanAttack_AttackHitsTheTarget(XMINT2 MapPos)
{
	// 全ての攻撃に対して
	for (int i = 0; i < m_Attack_Datas.size(); i++) {
		// 現在のスタミナで攻撃が可能か・その攻撃が当たるか
		if (m_Stamina >= m_Attack_Datas[i].m_Cost && AttackHitsTheTarget(MapPos, m_Attack_Datas[i])) {
			// 可能でかつ当たるならインデックスを返す
			return i;
		}
	}

	return -1;
}

void Enemy::StartTurnWhitchAct()
{
	m_GenocideFlag = false;
	// すでにいずれかの行動がプレイヤーユニットに当たる位置に居るか
	if (CanAttack_AttackHitsTheTarget(m_MapPos) != -1) {
		// 居るならそのまま攻撃する
		m_Act = AC_Attack;
		return;
	}


	// (仮) 即攻撃以外は移動スタートで
	m_Act = AC_Move;

	//------- どこに移動するか
	bool ConfirmRoute = false;
	
	//--------- プレイヤーを倒せる行動があるか、そしてその行動が可能であるか ---------//

	// 倒せる行動のインデックス ( 倒せる行動が存在しない場合は NOT_FOUND (-1)  )
	const int NOT_FOUND = -1;
	int CanKillActIndex = NOT_FOUND;

	// 全行動に対して
	for (int i = 0; i < m_Attack_Datas.size(); i++) {
		// その行動に対するスタミナが足りるか
		if (m_Stamina < m_Attack_Datas[i].m_Cost) {
			continue;
		}
		// 行動後のプレイヤーHP
		int AfterHP = pPlayer->GetHP() - (m_Attack_Datas[i].m_Damage - pPlayer->GetDef());
		// その行動で倒せるならフラグを true にする
		if (AfterHP <= 0) {
			CanKillActIndex = i;
		}
	}

	//--------- プレイヤーを倒せる行動が可能であるなら、その行動を実行できる位置に移動可能か ---------//
	// 現在位置
	XMINT2 Start = GetMapPos();
	// プレイヤー位置保存用
	XMINT2 PlayerPos = pPlayer->GetMapPos();

	// 倒せる行動が可能であるなら
	if (CanKillActIndex != NOT_FOUND) {
		// 倒せる行動の攻撃範囲
		int ActRange = m_Attack_Datas[CanKillActIndex].m_Range;
		
		// 最終移動位置
		m_Route.Cost = 999999;
		// 移動位置判定用バッファ
		AstarResults ResultBuff;

		// 移動後に行動が当たる範囲内のマス全てを探索、一番移動コストが低いものを保存
		for (int x = 0; x < ActRange; x++) {
			int Remainder = ActRange - x;
			for (int y = 0; y < Remainder; y++) {
				XMINT2 GoalPos = PlayerPos + XMINT2(x, y);

				// ルート一時保存
				ResultBuff = Astar::Instance()->DoAstar(Cell(Start.x, Start.y), Cell(GoalPos.x, GoalPos.y), m_pMap->CostTable);
				// 現在保存中のルートよりコストが低ければ更新する
				if (m_Route.Cost > ResultBuff.Cost) {
					m_Route = ResultBuff;
				}
			}
		}
		// 最短のルートが移動可能範囲にあるか
		if (m_MoveCost <= m_Route.Cost) {
			// 最終移動位置とルートを確定する
			ConfirmRoute = true;
			// 一撃で倒せる行動がある場合のフラグを立てる
			m_GenocideFlag = true;
			m_Route.route_list.pop_front();
			m_Route.route_list.pop_back();
		}
	}

	// まだルートが確定していないなら
	if (ConfirmRoute == false) {
		// 単純に最小コストルートを移動する
		m_Route = Astar::Instance()->DoAstar(Cell(Start.x, Start.y), Cell(PlayerPos.x, PlayerPos.y), m_pMap->CostTable);
		// 最後にあるプレイヤーのマスを削除する
		m_Route.route_list.pop_back();
		m_Route.route_list.pop_front();
		ConfirmRoute = true;

		//// ルートをvectorに変換
		//std::vector<Cell> Route(m_Route.route_list.begin(), m_Route.route_list.end());
		//// 一マスずつ移動(実際は移動してない)  攻撃が当たるまで
		//for (int i = 0; i < Route.size(); i++) {
		//	// 当たる行動があるなら
		//	if (CanAttack_AttackHitsTheTarget(XMINT2(Route[i].x, Route[i].y)) != -1) {

		//		ConfirmRoute = true;
		//	}
		//}
		
	}


	/*----------  関数内補足説明
	倒せる行動がある
	その行動を実行できるだけのスタミナがある
	実行可能な位置に移動可能である
	この 3 項目を満たした場合のみ倒せる行動を優先する


	確定行動以外は最小コストルートの移動中に
	攻撃可能になった攻撃を優先して行動する。

	----------*/
}

void Enemy::Act()
{
	if (m_CoolTime > 0) {
		m_CoolTime--;
	}

	if (m_Act == AC_Move && m_CoolTime == 0) {
		m_CoolTime = 15;
		std::vector<Cell> result(m_Route.route_list.begin(), m_Route.route_list.end());
		Cell p = result[0];
		// 次マスに移動できる
		if (m_pMap->CostTable[p.x][p.y] <= m_MoveCost) {
			// 移動コスト分コストを引く
			m_MoveCost -= m_pMap->CostTable[p.x][p.y];
			// 移動する
			SetMapPos(XMINT2(p.x, p.y));
			// 移動したのでルートを詰める
			m_Route.route_list.pop_front();
			// カメラの位置を更新する
			CameraTargetOnMe(true, WAIT_TIMER_VALUE_MOVE);

			// 現在のマスで攻撃ができる かつ　一撃で倒せる行動が無い
			if (CanAttack_AttackHitsTheTarget(m_MapPos) != -1 && m_GenocideFlag == false) {
				// 行動を攻撃に変更する
				m_Act = AC_Attack;
			}
			// 一撃で倒せる行動をする
			else if (m_Route.route_list.size() == 0 && m_GenocideFlag == true) {
				// 行動を攻撃に変更する
				m_Act = AC_Attack;
			}
			// もう移動できないし行動可能な攻撃も無いのでターンエンド
			else if (m_Route.route_list.size() == 0) {
				TurnEnd();
			}
		}
		// できないのでターンエンド
		else {
			TurnEnd();
		}
	}

	if (m_Act == AC_Attack && m_CoolTime == 0) {
		m_CoolTime = 15;

		// 行動のインデックスの初期値
		const int NOT_FOUND = -1;
		// 行動のインデックス
		int ActIndex = NOT_FOUND;

		// 攻撃後のHP保存
		int AfterHPBuffer = 9999;

		// 全行動に対して
		for (int i = 0; i < m_Attack_Datas.size(); i++) {
			// その行動に対するスタミナが足りるか
			if (m_Stamina < m_Attack_Datas[i].m_Cost) {
				continue;
			}
			// その攻撃が当たるか
			if (!AttackHitsTheTarget(m_MapPos, m_Attack_Datas[i])) {
				continue;
			}
			// 行動後のプレイヤーHP
			int AfterHP = pPlayer->GetHP() - (m_Attack_Datas[i].m_Damage - pPlayer->GetDef());

			// 一番ダメージの大きい行動を判定する

			// ActIndexが NOT_FOUND の場合はとりあえず入れる
			if (ActIndex == NOT_FOUND) {
				ActIndex = i;
				AfterHPBuffer = AfterHP;
			}
			// 最終ダメージが大きい方を保存する
			else if (AfterHP < AfterHPBuffer) {
				ActIndex = i;
				AfterHPBuffer = AfterHP;
			}
		}

		// あとでAttackActに書き変える
		// プレイヤーにダメージを与えてリアクションさせる
		//ActionEffectsMgr::Instance()->StartMagicEffect(GetModelPos(), pPlayer->GetModelPos());
		//Wait(ActionEffectsMgr::Instance()->MAGIC_EFFECT_TIME);

		m_isNowAttack = true;
		m_AttackIndex = ActIndex;
		AttackEffect();
		/*pPlayer->DamageShake(m_Attack_Datas[ActIndex].m_Damage);
		pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
		TurnEnd();*/
	}
}

void Enemy::TurnEnd()
{
	TurnEndTimer = 20;
	TurnEndFlag = true;


	/*m_MyTurn = false;
	m_Next = true;
	m_Act = AC_Wait;*/
}

void Enemy::AttackEffect() {
	m_AttackTimer = 0;
	if (m_Attack_Datas[m_AttackIndex].m_Name == "魔法 - 1") {
		XMFLOAT3 StartPos = GetModelPos();
		StartPos.y += 3.0f;
		ActionEffectsMgr::Instance()->StartMagicEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->MAGIC_EFFECT_TIME);
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "魔法 - 2") {
		XMFLOAT3 StartPos = GetModelPos();
		StartPos.y += 3.0f;
		ActionEffectsMgr::Instance()->StartMagicEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->MAGIC_EFFECT_TIME);
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "攻撃 - 1") {
		XMFLOAT3 StartPos = GetModelPos();
		ActionEffectsMgr::Instance()->StartNoveceEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->NOVICE_EFFECT_TIME);

		NoviceAttackReaction(GetMapPos(), pPlayer->GetMapPos());
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "攻撃 - 2") {
		XMFLOAT3 StartPos = GetModelPos();
		ActionEffectsMgr::Instance()->StartNoveceEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->NOVICE_EFFECT_TIME);

		NoviceAttackReaction(GetMapPos(), pPlayer->GetMapPos());
	}
}

void Enemy::AttackEffectUpdate() {
	m_AttackTimer++;
	if (m_Attack_Datas[m_AttackIndex].m_Name == "魔法 - 1") {
		// 待ち時間が終わった
		if (!isWait()) {
			m_isNowAttack = false;
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
			TurnEnd();
		}
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "魔法 - 2") {
		// 待ち時間が終わった
		if (!isWait()) {
			m_isNowAttack = false;
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
			TurnEnd();
		}
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "攻撃 - 1") {
		// 攻撃する
		if (m_AttackTimer == m_ReactionTrigger) {
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
		}
		// 待ち時間が終わった
		if (!isWait()) {
			m_isNowAttack = false;
			TurnEnd();
		}
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "攻撃 - 2") {
		// 攻撃する
		if (m_AttackTimer == m_ReactionTrigger) {
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
		}
		// 待ち時間が終わった
		if(!isWait()){
			m_isNowAttack = false;
			TurnEnd();
		}
	}
}