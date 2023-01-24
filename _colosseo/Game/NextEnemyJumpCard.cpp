#include "NextEnemyJumpCard.h"
#include "../Input.h"
#include "Player.h"
#include "Enemys.h"
#include "Cards.h"

NextEnemyJumpCard::NextEnemyJumpCard()
{
	ConstructInitialize();
	m_Type = CardType::NEXT_EN_JUMP;
	//Graph = TexManager::LoadTexture("Resource/image/MOVE_CARD.png");

	Graph = TexManager::GetColor(XMFLOAT4(255, 0, 0, 255));

	// 攻撃カードなので攻撃力を設定
	m_PreDamage = 0;

	// フェーズ数設定
	m_PheseCount = 3;
}

NextEnemyJumpCard::~NextEnemyJumpCard()
{
}

void NextEnemyJumpCard::Draw(int index, int MaxIdx)
{
	float width = 100.0f;
	float height = 200.0f;
	//XMFLOAT2 LeftTop;
	float CenterIdx = MaxIdx / 2.0f;
	//if (MaxIdx % 2 == 0) {
	LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f + 100.0f, WINDOW_HEIGHT - 200.0f/* + 150.0f */ };

	DrawGraph(LeftTop, LeftTop + XMFLOAT2(width, height), Graph);

	if (Active) {
		DrawGraph(LeftTop - XMFLOAT2(3, 3), LeftTop + XMFLOAT2(width, height) + XMFLOAT2(3, 3), TexManager::GetColor(ColorConvert2(XMFLOAT4(1, 0, 1, 0.5f))));
	}
	else if (HitCursor) {
		DrawGraph(LeftTop - XMFLOAT2(3, 3), LeftTop + XMFLOAT2(width, height) + XMFLOAT2(3, 3), TexManager::GetColor(ColorConvert2(XMFLOAT4(1, 1, 0, 0.5f))));
	}
}

#pragma region Phese1

void NextEnemyJumpCard::Phese1Start()
{

}

void NextEnemyJumpCard::UpdatePhese1()
{
	// フェーズ開始時限定処理
	if (m_PheseStartTriggerFlag) {
		m_PheseStartTriggerFlag = false;
		Phese1Start();
	}

	// カード選択時処理
	if (Input::isMouseTrigger(M_LEFT)) {
		// フェーズ即終了
		PheseEndFunc(0);
	}



	// フェーズ終了時限定処理
	if (CanPhaseEnd()) {
		m_WaitTimer = 0;
		m_PheseStartTriggerFlag = false;
		m_isPheseEnd = false;
		Phese1End();
	}
}

void NextEnemyJumpCard::Phese1End()
{
}

#pragma endregion

#pragma region Phese2

void NextEnemyJumpCard::Phese2Start()
{
}

void NextEnemyJumpCard::UpdatePhese2()
{
	// フェーズ開始時限定処理
	if (m_PheseStartTriggerFlag) {
		m_PheseStartTriggerFlag = false;
		Phese2Start();
	}

	if (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip) {
		// 選択一度目はカードを発動しない
		//if (m_PreCheckChip == m_NowCheckChip) {
		ChangePlayerDir(pPlayer);

		pPlayer->SetMapPos(m_NowCheckChip);
		GameCamera::Instance()->Targeting(pMap->ChangePos(m_NowCheckChip), GameCamera::Instance()->DEFAULT_FLAME_TIME);

		// フェーズ即終了
		PheseEndFunc(0);
		//}
	}

	// フェーズ終了時限定処理
	if (CanPhaseEnd()) {
		m_WaitTimer = 0;
		m_PheseStartTriggerFlag = false;
		m_isPheseEnd = false;
		Phese2End();
	}
}

void NextEnemyJumpCard::Phese2End()
{
}

#pragma endregion

#pragma region Phese3

void NextEnemyJumpCard::Phese3Start()
{
}

void NextEnemyJumpCard::UpdatePhese3()
{
	// フェーズ開始時限定処理
	if (m_PheseStartTriggerFlag) {
		m_PheseStartTriggerFlag = false;
		Phese3Start();
	}

	if (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip) {
		// 選択一度目はカードを発動しない
		//if (m_PreCheckChip == m_NowCheckChip) {
		ChangePlayerDir(pPlayer);

		//pPlayer->SetMapPos(m_NowCheckChip);
		//GameCamera::Instance()->Targeting(pMap->ChangePos(m_NowCheckChip), GameCamera::Instance()->DEFAULT_FLAME_TIME);

		// フェーズ即終了
		PheseEndFunc(0);
		// }
	}

	// フェーズ終了時限定処理
	if (CanPhaseEnd()) {
		m_WaitTimer = 0;
		m_PheseStartTriggerFlag = false;
		m_isPheseEnd = false;
		Phese3End();
	}
}

void NextEnemyJumpCard::Phese3End()
{
}

void NextEnemyJumpCard::SelectTrigger(XMINT2 RefPoint, Cards* pCards)
{
}

#pragma endregion

void NextEnemyJumpCard::SelectTriggerEn(XMINT2 RefPoint, std::vector<XMINT2> TargetPos, Cards* pCards)
{
	for (auto& data : TargetPos) {
		// 選択可能マスを追加する
		pCards->AddCanSelectChips_Move(data + XMINT2(1, 0));
		pCards->AddCanSelectChips_Move(data + XMINT2(-1, 0));
		pCards->AddCanSelectChips_Move(data + XMINT2(0, 1));
		pCards->AddCanSelectChips_Move(data + XMINT2(0, -1));
	}

}

void NextEnemyJumpCard::ChangePlayerDir(Player* _pPlayer)
{
	// 攻撃した方向に向きを変更
	if (m_NowCheckChip.x == pPlayer->GetMapPos().x) {
		if (m_NowCheckChip.y > pPlayer->GetMapPos().y) {
			// 向きを設定
			pPlayer->SetDir(Charactor::Chara_Dir::BACK);
		}
		else if (m_NowCheckChip.y < pPlayer->GetMapPos().y) {
			// 向きを設定
			pPlayer->SetDir(Charactor::Chara_Dir::FRONT);
		}
	}
	else {
		if (m_NowCheckChip.x > pPlayer->GetMapPos().x) {
			// 向きを設定
			pPlayer->SetDir(Charactor::Chara_Dir::RINGHT);
		}
		else if (m_NowCheckChip.x < pPlayer->GetMapPos().x) {
			// 向きを設定
			pPlayer->SetDir(Charactor::Chara_Dir::LEFT);
		}
	}
}

void NextEnemyJumpCard::SelectTrigger_Dir(XMINT2 RefPoint, Cards* pCards)
{
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, 0));
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, 0));
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, 1));
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, -1));
}

void NextEnemyJumpCard::SetSelectData(XMINT2 _Now, XMINT2 _Pre)
{
	// 現在の選択マスをセット
	m_NowCheckChip = _Now;
	// 前回の選択マスをセット
	m_PreCheckChip = _Pre;
}
