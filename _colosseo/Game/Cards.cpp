#include "Cards.h"
#include "..\Input.h"
#include "GameCamera.h"

#include "NormalMoveCard.h"
#include "NormalAttackCard.h"

Cards::Cards()
{

	//m_Cards.reserve(16);
	/*m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));*/


	//m_Cards.push_front(Card(CardType::NONE));

	m_UseCardType = CardType::NONE;
	NowPhase = 0;
	TurnEnd = false;

	TurnEndButton = TexManager::LoadTexture("Resource/image/TurnEnd.png");

	LT = XMFLOAT2(WINDOW_WIDTH - 300, WINDOW_HEIGHT - 200);

	isMyTurn = false;


	m_Cards.reserve(16);
}

Cards::~Cards()
{

}

void Cards::Initialize(Player *_Player, Enemys *_Enemys, Map *_Map)
{
	pPlayer = _Player;
	pEnemys = _Enemys;
	pMap = _Map;
	/*m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));*/

	for (int i = 0; i < int(m_Cards.size()); i++) {
		delete m_Cards[i];
	}
	m_Cards.clear();

	m_Cards.emplace_back(new NormalAttackCard());
	m_Cards.emplace_back(new NormalAttackCard());
	m_Cards.emplace_back(new NormalMoveCard());
	m_Cards.emplace_back(new NormalMoveCard());
	m_Cards.emplace_back(new NormalMoveCard());

	m_Cards[0]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[1]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[2]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[3]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[4]->SetPlEnPtr(pPlayer, pEnemys, pMap);

	m_Cards[0]->Initialize(0, 5);
	m_Cards[1]->Initialize(1, 5);
	m_Cards[2]->Initialize(2, 5);
	m_Cards[3]->Initialize(3, 5);
	m_Cards[4]->Initialize(4, 5);

	/*for (int i = 0; i < 5; i++) {
		m_Cards[i]->Initialize(i, 5);
	}*/
}

void Cards::Update()
{
	if (WaitTimer > 0) {
		WaitTimer--;
	}
	for (int i = 0; i < int(m_Cards.size()); i++) {
		m_Cards[i]->Update(i, int(m_Cards.size()));
	}
	for (int i = 0; i < m_Cards.size(); i++) {
		if (GCM::Instance()->CheckCollision(m_Cards[i]->collider, "Mouse")) {
			m_Cards[i]->HitCursor = true;
			if (Input::isMouseTrigger(M_LEFT)) {
				//m_Cards.(m_Cards.begin() + i);
				//m_Cards.erase_after(std::next(m_Cards.before_begin(), i));
			}
		}
		else {
			m_Cards[i]->HitCursor = false;
		}
	}
}

void Cards::Draw()
{
	for (int i = 0; i < int(m_Cards.size()); i++) {
		m_Cards[i]->Draw(i, int(m_Cards.size()));
	}

	if (m_UseCardType != CardType::NONE) {
		switch (m_UseCardType)
		{
		case CardType::DEFAULT_ATTACK:
			for (auto& data : CanSelectChips) {
				pMap->DrawGuide(data, ColorConvert(255, 0, 0, 255), GuidePriority::MOVE_OR_ATTACK, false);
			}
			if (PreChipExistEnemy) {
				pMap->DrawGuide(PreCheckChipOld, ColorConvert(218, 82, 58, 255), GuidePriority::SELECT_ENEMY_PRE_CARD_USE, true);
			}
			break;
		case CardType::DEFAULT_MOVE:
			for (auto& data : CanSelectChips) {
				pMap->DrawGuide(data, ColorConvert(0, 183, 206, 255), GuidePriority::MOVE_OR_ATTACK, false);
			}
			break;
		default:
			break;
		}

	}
	else if(isMyTurn) {
		if (WaitTimer == 0) {
			DrawGraph(LT,
				LT + XMFLOAT2(160, 64) * 1.5f,
				TurnEndButton);
		}
		else {
			DrawGraphEffColor(LT,
				LT + XMFLOAT2(160, 64) * 1.5f,
				TurnEndButton, XMFLOAT4(0.3f, 0.3f, 0.4f, 1.0f));
		}
		XMFLOAT2 point = Input::GetMousePos();

		if (point.x >= LT.x && point.x <= LT.x + 160 * 1.5f &&
			point.y >= LT.y && point.y <= LT.y + 64 * 1.5f) {
			DrawGraph(LT - XMFLOAT2(2, 2),
				LT + XMFLOAT2(160, 64) * 1.5f + XMFLOAT2(2, 2),
				TexManager::GetColor(ColorConvert2(XMFLOAT4(1, 1, 0, 0.5f))));
		}
	}

	// 方向決定時
	if (NowPhase == 2) {
		for (auto& data : CanSelectChips) {
			pMap->DrawGuide(data, ColorConvert(0, 183, 206, 255), GuidePriority::MOVE_OR_ATTACK, false);
		}
	}

	if (isMyTurn) {
		pMap->DrawGuide(pPlayer->GetMapPos(), ColorConvert(171, 225, 250, 255), GuidePriority::NOW_ACT_UNIT);
	}
}

void Cards::StartTurn()
{
	for (int i = 0; i < int(m_Cards.size()); i++) {
		delete m_Cards[i];
	}

	m_Cards.clear();
	/*m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));*/

	m_Cards.emplace_back(new NormalAttackCard());
	m_Cards.emplace_back(new NormalAttackCard());
	m_Cards.emplace_back(new NormalMoveCard());
	m_Cards.emplace_back(new NormalMoveCard());
	m_Cards.emplace_back(new NormalMoveCard());

	m_Cards[0]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[1]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[2]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[3]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[4]->SetPlEnPtr(pPlayer, pEnemys, pMap);

	m_Cards[0]->Initialize(0, 5);
	m_Cards[1]->Initialize(1, 5);
	m_Cards[2]->Initialize(2, 5);
	m_Cards[3]->Initialize(3, 5);
	m_Cards[4]->Initialize(4, 5);


	m_UseCardType = CardType::NONE;
	NowPhase = 0;
	for (int i = 0; i < int(m_Cards.size()); i++) {
		m_Cards[i]->Initialize(i, int(m_Cards.size()));
	}
	TurnEnd = false;

	isMyTurn = true;
}

bool Cards::ExistInCanSelectChips(XMINT2 MapPos)
{
	bool Flag = false;
	for (auto &data : CanSelectChips) {
		if (data == MapPos) {
			Flag = true;
		}
	}
	return Flag;
}

void Cards::AddCanSelectChips(XMINT2 MapPos)
{
	if (pMap->InMap(MapPos)) {
		CanSelectChips.push_back(MapPos);
	}
}

void Cards::AddCanSelectChips_Move(XMINT2 MapPos)
{
	if (pMap->InMap(MapPos) && pMap->CostTable[MapPos.x][MapPos.y] < 999) {
		CanSelectChips.push_back(MapPos);
	}
}

void Cards::PlayerTurnUpdate()
{
	if (NowPhase == 0) {
		CardPhaseUpdate();
	}
	else if (NowPhase == 1) {
		CardEffectPhaseUpdate();
	}
	else if (NowPhase == 2) {
		CardAfterPhaseUpdate();
	}
}

void Cards::CardPhaseUpdate()
{
	if (m_UseCardType == CardType::NONE) {
		for (int i = 0; i < m_Cards.size(); i++) {
			if (GCM::Instance()->CheckCollision(m_Cards[i]->collider, "Mouse")) {
				if (Input::isMouseTrigger(M_LEFT)) {
					m_UseCardType = m_Cards[i]->GetType();
					m_UseCardIndex = i;
					m_Cards[i]->Active = true;
					m_NowSelectChip = pPlayer->GetMapPos();
					PreCheckChipOld = XMINT2(-1, -1);
					PreCheckChip = XMINT2(-2, -2);

					// 選択カードのアップデート
					if (m_UseCardType == CardType::DEFAULT_ATTACK) {
						m_Cards[i]->SelectTrigger(pPlayer->GetMapPos(), this);
						PreDamage = m_Cards[i]->m_PreDamage;
					}
					else if (m_UseCardType == CardType::DEFAULT_MOVE) {
						m_Cards[i]->SelectTrigger(pPlayer->GetMapPos(), this);
						PreDamage = m_Cards[i]->m_PreDamage;
						for (int x = 0; x < CanSelectChips.size(); x++) {
							if (pEnemys->GetEnemy(CanSelectChips[x]) != nullptr) {
								CanSelectChips.erase(CanSelectChips.begin() + x);
							}
						}
					}
					m_Cards[i]->SetSelectData(m_NowSelectChip, PreCheckChip);
					m_Cards[i]->UpdatePhese1();


					/*if (m_UseCardType == CardType::DEFAULT_ATTACK) {
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, 1));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, -1));
						PreDamage = 1;
					}
					else if (m_UseCardType == CardType::DEFAULT_MOVE) {
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(1, 0));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-1, 0));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, 1));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, -1));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(2, 0));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-2, 0));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, 2));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, -2));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(1, 1));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-1, 1));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-1, -1));
						AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(1, -1));
						PreDamage = 0;

						for (int x = 0; x < CanSelectChips.size(); x++){
							if (pEnemys->GetEnemy(CanSelectChips[x]) != nullptr) {
								CanSelectChips.erase(CanSelectChips.begin() + x);
							}
						}
					}*/

					break;
				}
			}
		}
	}

	// フェーズエンドフラグが立っているなら
	if (m_UseCardIndex != -1) {
		if (m_Cards[m_UseCardIndex]->m_PheseChange) {
			m_Cards[m_UseCardIndex]->m_PheseChange = false;
			NowPhase = 1;
		}
	}

	// Waitが0でない間はターンエンドできない
	if (WaitTimer == 0 ) {
		XMFLOAT2 point = Input::GetMousePos();
		if (point.x >= LT.x && point.x <= LT.x + 160 * 1.5f &&
			point.y >= LT.y && point.y <= LT.y + 64 * 1.5f) {
			if (Input::isMouseTrigger(M_LEFT) && pEnemys->GetEnemyCount() != 0) {
				TurnEnd = true;
				isMyTurn = false;
			}
		}
	}
	/*if (Input::isKeyTrigger(DIK_RETURN)) {
		TurnEnd = true;
	}*/
}

void Cards::CardEffectPhaseUpdate()
{
	if (Input::isKeyTrigger(DIK_BACKSPACE) || Input::isMouseTrigger(M_RIGHT)) {
		m_UseCardType = CardType::NONE;
		m_Cards[m_UseCardIndex]->Active = false;
		NowPhase = 0;
		CanSelectChips.clear();
		PreDamage = 0;
		PreCheckChipOld = XMINT2(-1, -1);
		PreCheckChip = XMINT2(-2, -2);

		m_UseCardIndex = -1;
	}
	if (Input::GetMouseMove() == Input::MouseMove((LONG)0, (LONG)0, (LONG)0)) {
		if (Input::isKeyTrigger(DIK_UP)) {
			m_NowSelectChip.y++;
		}
		if (Input::isKeyTrigger(DIK_DOWN)) {
			m_NowSelectChip.y--;
		}
		if (Input::isKeyTrigger(DIK_LEFT)) {
			m_NowSelectChip.x--;
		}
		if (Input::isKeyTrigger(DIK_RIGHT)) {
			m_NowSelectChip.x++;
		}
	}
	else {
		m_NowSelectChip = pMap->NowHitChip;
	}


	// 選択中の

	if (m_UseCardType == CardType::DEFAULT_ATTACK) {
		m_Cards[m_UseCardIndex]->SetSelectData(m_NowSelectChip, PreCheckChip);
		m_Cards[m_UseCardIndex]->UpdatePhese2();
		//en->Damage(1);
		//m_UseCardType = CardType::NONE;
		//PreDamage = 0;
		////NowPhase = 0;
		//CanSelectChips.clear();
		//PreCheckChipOld = XMINT2(-1, -1);
		//PreCheckChip = XMINT2(-2, -2);
		//en->DamageReaction(pPlayer->GetMapPos(), en->GetMapPos());
		Wait(10);


		//if (Input::isKeyTrigger(DIK_RETURN) || (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip)) {
		//	// 選択一度目はカードを発動しない
		//	if (PreCheckChipOld != PreCheckChip) {
		//		PreCheckChipOld = PreCheckChip;
		//		Enemy* en = pEnemys->GetEnemy(m_NowSelectChip);
		//		if (en) PreChipExistEnemy = true;
		//		else PreChipExistEnemy = false;
		//	}
		//	else {
		//		Enemy* en = pEnemys->GetEnemy(m_NowSelectChip);
		//		if (en != nullptr && ExistInCanSelectChips(m_NowSelectChip)) {
		//			// 攻撃した方向に向きを変更
		//			if (m_NowSelectChip.x == pPlayer->GetMapPos().x) {
		//				if (m_NowSelectChip.y > pPlayer->GetMapPos().y) {
		//					// 向きを設定
		//					pPlayer->SetDir(Charactor::Chara_Dir::BACK);
		//				}
		//				else if (m_NowSelectChip.y < pPlayer->GetMapPos().y) {
		//					// 向きを設定
		//					pPlayer->SetDir(Charactor::Chara_Dir::FRONT);
		//				}
		//			}
		//			else {
		//				if (m_NowSelectChip.x > pPlayer->GetMapPos().x) {
		//					// 向きを設定
		//					pPlayer->SetDir(Charactor::Chara_Dir::RINGHT);
		//				}
		//				else if (m_NowSelectChip.x < pPlayer->GetMapPos().x) {
		//					// 向きを設定
		//					pPlayer->SetDir(Charactor::Chara_Dir::LEFT);
		//				}
		//			}
		//			//
		//			en->Damage(1);
		//			m_Cards.erase_after(std::next(m_Cards.before_begin(), m_UseCardIndex));
		//			m_UseCardType = CardType::NONE;
		//			PreDamage = 0;
		//			NowPhase = 0;
		//			CanSelectChips.clear();
		//			PreCheckChipOld = XMINT2(-1, -1);
		//			PreCheckChip = XMINT2(-2, -2);
		//			en->DamageReaction(pPlayer->GetMapPos(), en->GetMapPos());
		//			Wait(10);
		//		}
		//	}
		//}
	}
	else if (m_UseCardType == CardType::DEFAULT_MOVE) {
		if (Input::isKeyTrigger(DIK_RETURN) || (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip)) {
			// 選択したマスが移動可能なら
			if (pMap->InMap(m_NowSelectChip) && ExistInCanSelectChips(m_NowSelectChip)) {
				m_Cards[m_UseCardIndex]->SetSelectData(m_NowSelectChip, PreCheckChip);
				m_Cards[m_UseCardIndex]->UpdatePhese2();
				//m_Cards.erase_after(std::next(m_Cards.before_begin(), m_UseCardIndex));

				//pPlayer->SetMapPos(m_NowSelectChip);
				//m_UseCardType = CardType::NONE;
				PreDamage = 0;
				//NowPhase = 0;
				CanSelectChips.clear();
				//GameCamera::Instance()->Targeting(pMap->ChangePos(m_NowSelectChip), GameCamera::Instance()->DEFAULT_FLAME_TIME);
				PreCheckChipOld = XMINT2(-1, -1);
				PreCheckChip = XMINT2(-2, -2);

				// 移動後に向きを変更できる
				AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, 0));
				AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, 0));
				AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, 1));
				AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, -1));
				//NowPhase = 2;
			}
		}
	}

	// 各カード処理用のパラメータ更新
	if (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip) {
		// 前回どのマスを選択していたか
		PreCheckChip = m_NowSelectChip;
	}

	// フェーズエンドフラグが立っているなら
	if (m_UseCardIndex != -1) {
		if (m_Cards[m_UseCardIndex]->m_PheseChange) {
			m_Cards[m_UseCardIndex]->m_PheseChange = false;
			NowPhase = 2;
			if (m_Cards[m_UseCardIndex]->m_PheseCount == NowPhase) {
				NowPhase = 0;
				//m_Cards.erase_after(std::next(m_Cards.before_begin(), m_UseCardIndex));
				delete m_Cards[m_UseCardIndex];
				m_Cards.erase(m_Cards.begin() + m_UseCardIndex);
				m_UseCardType = CardType::NONE;
				PreDamage = 0;
				CanSelectChips.clear();
				PreCheckChipOld = XMINT2(-1, -1);
				PreCheckChip = XMINT2(-2, -2);
			}
		}
	}

	//GameBaseCollider *collider;
	//
	//// collider -> 自分のCollider
	//if (GCM::Instance()->CheckCollision(collider, "TargetTag")) {
	//	// TargetTagの付いているColliderと衝突した
	//}

}

void Cards::CardAfterPhaseUpdate()
{
	// 現在は向き変更のみ対応
	// 選択マスの移動
	if (Input::GetMouseMove() == Input::MouseMove((LONG)0, (LONG)0, (LONG)0)) {
		if (Input::isKeyTrigger(DIK_UP)) {
			m_NowSelectChip.y++;
		}
		if (Input::isKeyTrigger(DIK_DOWN)) {
			m_NowSelectChip.y--;
		}
		if (Input::isKeyTrigger(DIK_LEFT)) {
			m_NowSelectChip.x--;
		}
		if (Input::isKeyTrigger(DIK_RIGHT)) {
			m_NowSelectChip.x++;
		}
	}
	else {
		m_NowSelectChip = pMap->NowHitChip;
	}


	if (m_UseCardType == CardType::DEFAULT_MOVE) {
		m_Cards[m_UseCardIndex]->SetSelectData(m_NowSelectChip, PreCheckChip);
		m_Cards[m_UseCardIndex]->UpdatePhese3();
	}

	// 向きを決定
	//if (Input::isKeyTrigger(DIK_RETURN) || (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip)) {
	//	// 選択マスが対象範囲内か
	//	if (pMap->InMap(m_NowSelectChip) && ExistInCanSelectChips(m_NowSelectChip)) {
	//		// 範囲内なら
	//		if (m_NowSelectChip.x == pPlayer->GetMapPos().x) {
	//			if (m_NowSelectChip.y > pPlayer->GetMapPos().y) {
	//				// 向きを設定
	//				pPlayer->SetDir(Charactor::Chara_Dir::BACK);
	//			}
	//			else if (m_NowSelectChip.y < pPlayer->GetMapPos().y) {
	//				// 向きを設定
	//				pPlayer->SetDir(Charactor::Chara_Dir::FRONT);
	//			}
	//		}
	//		else {
	//			if (m_NowSelectChip.x > pPlayer->GetMapPos().x) {
	//				// 向きを設定
	//				pPlayer->SetDir(Charactor::Chara_Dir::RINGHT);
	//			}
	//			else if (m_NowSelectChip.x < pPlayer->GetMapPos().x) {
	//				// 向きを設定
	//				pPlayer->SetDir(Charactor::Chara_Dir::LEFT);
	//			}
	//		}
	//		// 終了
	//		//NowPhase = 0;
	//	}

	//}


	// フェーズエンドフラグが立っているなら
	if (m_UseCardIndex != -1) {
		if (m_Cards[m_UseCardIndex]->m_PheseChange) {
			m_Cards[m_UseCardIndex]->m_PheseChange = false;
			NowPhase = 0;
			//m_Cards.erase_after(std::next(m_Cards.before_begin(), m_UseCardIndex));

			delete m_Cards[m_UseCardIndex];
			m_Cards.erase(m_Cards.begin() + m_UseCardIndex);

			m_UseCardType = CardType::NONE;
			/*if (m_Cards[m_UseCardIndex]->m_PheseCount == NowPhase) {
				NowPhase = 0;
				m_Cards.erase_after(std::next(m_Cards.before_begin(), m_UseCardIndex));
			}*/
		}
	}
}

void Cards::Reset()
{
	for (int i = 0; i < int(m_Cards.size()); i++) {
		delete m_Cards[i];
	}

	m_Cards.clear();
	/*m_Cards.emplace_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.emplace_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.emplace_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.emplace_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.emplace_front(Card(CardType::DEFAULT_MOVE));*/

	m_Cards.emplace_back(new NormalAttackCard());
	m_Cards.emplace_back(new NormalAttackCard());
	m_Cards.emplace_back(new NormalMoveCard());
	m_Cards.emplace_back(new NormalMoveCard());
	m_Cards.emplace_back(new NormalMoveCard());

	m_Cards[0]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[1]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[2]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[3]->SetPlEnPtr(pPlayer, pEnemys, pMap);
	m_Cards[4]->SetPlEnPtr(pPlayer, pEnemys, pMap);

	m_Cards[0]->Initialize(0, 5);
	m_Cards[1]->Initialize(1, 5);
	m_Cards[2]->Initialize(2, 5);
	m_Cards[3]->Initialize(3, 5);
	m_Cards[4]->Initialize(4, 5);

	for (int i = 0; i < int(m_Cards.size()); i++) {
		m_Cards[i]->Initialize(i, int(m_Cards.size()));
	}

	m_UseCardType = CardType::NONE;
	NowPhase = 0;
	TurnEnd = false;

	LT = XMFLOAT2(WINDOW_WIDTH - 300, WINDOW_HEIGHT - 200);

	isMyTurn = false;
}
