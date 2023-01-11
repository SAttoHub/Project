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

	// ���o��ɍU������
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
	// �U�������𔻒�
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
	// �Ώۂ̈ʒu
	XMINT2 PlayerPos = pPlayer->GetMapPos();

	// �����I�ȍU���̏ꍇ�A������Ƀ^�[�Q�b�g�����Ȃ��Ɠ�����Ȃ�
	if (_Attack_Data.m_IsLinear == true &&
		(PlayerPos.x != MapPos.x && PlayerPos.y != MapPos.y)) {
		return false;
	}

	// �덷�C���p�I�t�Z�b�g
	double offset = 0.1;
	// �U���ΏۂƎ����̋��������߂� ( �덷�΍�ɃI�t�Z�b�g������ )
	double dist = sqrt(pow(MapPos.x - PlayerPos.x, 2) + pow(MapPos.y - PlayerPos.y, 2)) - offset;

	// �U���\�͈͓��ɑΏۂ�����Ȃ�U�����Ώۂɓ�����
	if (double(_Attack_Data.m_Range) >= dist) {
		return true;
	}

	// �U�����Ώۂɓ�����Ȃ�
	return false;
}

int Enemy::CanAttack_AttackHitsTheTarget(XMINT2 MapPos)
{
	// �S�Ă̍U���ɑ΂���
	for (int i = 0; i < m_Attack_Datas.size(); i++) {
		// ���݂̃X�^�~�i�ōU�����\���E���̍U���������邩
		if (m_Stamina >= m_Attack_Datas[i].m_Cost && AttackHitsTheTarget(MapPos, m_Attack_Datas[i])) {
			// �\�ł�������Ȃ�C���f�b�N�X��Ԃ�
			return i;
		}
	}

	return -1;
}

void Enemy::StartTurnWhitchAct()
{
	m_GenocideFlag = false;
	// ���łɂ����ꂩ�̍s�����v���C���[���j�b�g�ɓ�����ʒu�ɋ��邩
	if (CanAttack_AttackHitsTheTarget(m_MapPos) != -1) {
		// ����Ȃ炻�̂܂܍U������
		m_Act = AC_Attack;
		return;
	}


	// (��) ���U���ȊO�͈ړ��X�^�[�g��
	m_Act = AC_Move;

	//------- �ǂ��Ɉړ����邩
	bool ConfirmRoute = false;
	
	//--------- �v���C���[��|����s�������邩�A�����Ă��̍s�����\�ł��邩 ---------//

	// �|����s���̃C���f�b�N�X ( �|����s�������݂��Ȃ��ꍇ�� NOT_FOUND (-1)  )
	const int NOT_FOUND = -1;
	int CanKillActIndex = NOT_FOUND;

	// �S�s���ɑ΂���
	for (int i = 0; i < m_Attack_Datas.size(); i++) {
		// ���̍s���ɑ΂���X�^�~�i������邩
		if (m_Stamina < m_Attack_Datas[i].m_Cost) {
			continue;
		}
		// �s����̃v���C���[HP
		int AfterHP = pPlayer->GetHP() - (m_Attack_Datas[i].m_Damage - pPlayer->GetDef());
		// ���̍s���œ|����Ȃ�t���O�� true �ɂ���
		if (AfterHP <= 0) {
			CanKillActIndex = i;
		}
	}

	//--------- �v���C���[��|����s�����\�ł���Ȃ�A���̍s�������s�ł���ʒu�Ɉړ��\�� ---------//
	// ���݈ʒu
	XMINT2 Start = GetMapPos();
	// �v���C���[�ʒu�ۑ��p
	XMINT2 PlayerPos = pPlayer->GetMapPos();

	// �|����s�����\�ł���Ȃ�
	if (CanKillActIndex != NOT_FOUND) {
		// �|����s���̍U���͈�
		int ActRange = m_Attack_Datas[CanKillActIndex].m_Range;
		
		// �ŏI�ړ��ʒu
		m_Route.Cost = 999999;
		// �ړ��ʒu����p�o�b�t�@
		AstarResults ResultBuff;

		// �ړ���ɍs����������͈͓��̃}�X�S�Ă�T���A��Ԉړ��R�X�g���Ⴂ���̂�ۑ�
		for (int x = 0; x < ActRange; x++) {
			int Remainder = ActRange - x;
			for (int y = 0; y < Remainder; y++) {
				XMINT2 GoalPos = PlayerPos + XMINT2(x, y);

				// ���[�g�ꎞ�ۑ�
				ResultBuff = Astar::Instance()->DoAstar(Cell(Start.x, Start.y), Cell(GoalPos.x, GoalPos.y), m_pMap->CostTable);
				// ���ݕۑ����̃��[�g���R�X�g���Ⴏ��΍X�V����
				if (m_Route.Cost > ResultBuff.Cost) {
					m_Route = ResultBuff;
				}
			}
		}
		// �ŒZ�̃��[�g���ړ��\�͈͂ɂ��邩
		if (m_MoveCost <= m_Route.Cost) {
			// �ŏI�ړ��ʒu�ƃ��[�g���m�肷��
			ConfirmRoute = true;
			// �ꌂ�œ|����s��������ꍇ�̃t���O�𗧂Ă�
			m_GenocideFlag = true;
			m_Route.route_list.pop_front();
			m_Route.route_list.pop_back();
		}
	}

	// �܂����[�g���m�肵�Ă��Ȃ��Ȃ�
	if (ConfirmRoute == false) {
		// �P���ɍŏ��R�X�g���[�g���ړ�����
		m_Route = Astar::Instance()->DoAstar(Cell(Start.x, Start.y), Cell(PlayerPos.x, PlayerPos.y), m_pMap->CostTable);
		// �Ō�ɂ���v���C���[�̃}�X���폜����
		m_Route.route_list.pop_back();
		m_Route.route_list.pop_front();
		ConfirmRoute = true;

		//// ���[�g��vector�ɕϊ�
		//std::vector<Cell> Route(m_Route.route_list.begin(), m_Route.route_list.end());
		//// ��}�X���ړ�(���ۂ͈ړ����ĂȂ�)  �U����������܂�
		//for (int i = 0; i < Route.size(); i++) {
		//	// ������s��������Ȃ�
		//	if (CanAttack_AttackHitsTheTarget(XMINT2(Route[i].x, Route[i].y)) != -1) {

		//		ConfirmRoute = true;
		//	}
		//}
		
	}


	/*----------  �֐����⑫����
	�|����s��������
	���̍s�������s�ł��邾���̃X�^�~�i������
	���s�\�Ȉʒu�Ɉړ��\�ł���
	���� 3 ���ڂ𖞂������ꍇ�̂ݓ|����s����D�悷��


	�m��s���ȊO�͍ŏ��R�X�g���[�g�̈ړ�����
	�U���\�ɂȂ����U����D�悵�čs������B

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
		// ���}�X�Ɉړ��ł���
		if (m_pMap->CostTable[p.x][p.y] <= m_MoveCost) {
			// �ړ��R�X�g���R�X�g������
			m_MoveCost -= m_pMap->CostTable[p.x][p.y];
			// �ړ�����
			SetMapPos(XMINT2(p.x, p.y));
			// �ړ������̂Ń��[�g���l�߂�
			m_Route.route_list.pop_front();
			// �J�����̈ʒu���X�V����
			CameraTargetOnMe(true, WAIT_TIMER_VALUE_MOVE);

			// ���݂̃}�X�ōU�����ł��� ���@�ꌂ�œ|����s��������
			if (CanAttack_AttackHitsTheTarget(m_MapPos) != -1 && m_GenocideFlag == false) {
				// �s�����U���ɕύX����
				m_Act = AC_Attack;
			}
			// �ꌂ�œ|����s��������
			else if (m_Route.route_list.size() == 0 && m_GenocideFlag == true) {
				// �s�����U���ɕύX����
				m_Act = AC_Attack;
			}
			// �����ړ��ł��Ȃ����s���\�ȍU���������̂Ń^�[���G���h
			else if (m_Route.route_list.size() == 0) {
				TurnEnd();
			}
		}
		// �ł��Ȃ��̂Ń^�[���G���h
		else {
			TurnEnd();
		}
	}

	if (m_Act == AC_Attack && m_CoolTime == 0) {
		m_CoolTime = 15;

		// �s���̃C���f�b�N�X�̏����l
		const int NOT_FOUND = -1;
		// �s���̃C���f�b�N�X
		int ActIndex = NOT_FOUND;

		// �U�����HP�ۑ�
		int AfterHPBuffer = 9999;

		// �S�s���ɑ΂���
		for (int i = 0; i < m_Attack_Datas.size(); i++) {
			// ���̍s���ɑ΂���X�^�~�i������邩
			if (m_Stamina < m_Attack_Datas[i].m_Cost) {
				continue;
			}
			// ���̍U���������邩
			if (!AttackHitsTheTarget(m_MapPos, m_Attack_Datas[i])) {
				continue;
			}
			// �s����̃v���C���[HP
			int AfterHP = pPlayer->GetHP() - (m_Attack_Datas[i].m_Damage - pPlayer->GetDef());

			// ��ԃ_���[�W�̑傫���s���𔻒肷��

			// ActIndex�� NOT_FOUND �̏ꍇ�͂Ƃ肠���������
			if (ActIndex == NOT_FOUND) {
				ActIndex = i;
				AfterHPBuffer = AfterHP;
			}
			// �ŏI�_���[�W���傫������ۑ�����
			else if (AfterHP < AfterHPBuffer) {
				ActIndex = i;
				AfterHPBuffer = AfterHP;
			}
		}

		// ���Ƃ�AttackAct�ɏ����ς���
		// �v���C���[�Ƀ_���[�W��^���ă��A�N�V����������
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
	if (m_Attack_Datas[m_AttackIndex].m_Name == "���@ - 1") {
		XMFLOAT3 StartPos = GetModelPos();
		StartPos.y += 3.0f;
		ActionEffectsMgr::Instance()->StartMagicEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->MAGIC_EFFECT_TIME);
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "���@ - 2") {
		XMFLOAT3 StartPos = GetModelPos();
		StartPos.y += 3.0f;
		ActionEffectsMgr::Instance()->StartMagicEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->MAGIC_EFFECT_TIME);
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "�U�� - 1") {
		XMFLOAT3 StartPos = GetModelPos();
		ActionEffectsMgr::Instance()->StartNoveceEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->NOVICE_EFFECT_TIME);

		NoviceAttackReaction(GetMapPos(), pPlayer->GetMapPos());
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "�U�� - 2") {
		XMFLOAT3 StartPos = GetModelPos();
		ActionEffectsMgr::Instance()->StartNoveceEffect(StartPos, pPlayer->GetModelPos());
		Wait(ActionEffectsMgr::Instance()->NOVICE_EFFECT_TIME);

		NoviceAttackReaction(GetMapPos(), pPlayer->GetMapPos());
	}
}

void Enemy::AttackEffectUpdate() {
	m_AttackTimer++;
	if (m_Attack_Datas[m_AttackIndex].m_Name == "���@ - 1") {
		// �҂����Ԃ��I�����
		if (!isWait()) {
			m_isNowAttack = false;
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
			TurnEnd();
		}
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "���@ - 2") {
		// �҂����Ԃ��I�����
		if (!isWait()) {
			m_isNowAttack = false;
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
			TurnEnd();
		}
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "�U�� - 1") {
		// �U������
		if (m_AttackTimer == m_ReactionTrigger) {
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
		}
		// �҂����Ԃ��I�����
		if (!isWait()) {
			m_isNowAttack = false;
			TurnEnd();
		}
	}
	else if (m_Attack_Datas[m_AttackIndex].m_Name == "�U�� - 2") {
		// �U������
		if (m_AttackTimer == m_ReactionTrigger) {
			pPlayer->DamageShake(m_Attack_Datas[m_AttackIndex].m_Damage);
			pPlayer->DamageReaction(GetMapPos(), pPlayer->GetMapPos());
		}
		// �҂����Ԃ��I�����
		if(!isWait()){
			m_isNowAttack = false;
			TurnEnd();
		}
	}
}