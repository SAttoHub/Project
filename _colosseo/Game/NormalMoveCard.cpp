#include "NormalMoveCard.h"
#include "../Input.h"
#include "Player.h"
#include "Enemys.h"
#include "Cards.h"

NormalMoveCard::NormalMoveCard()
{
	ConstructInitialize();
	m_Type = CardType::DEFAULT_MOVE;
	Graph = TexManager::LoadTexture("Resource/image/MOVE_CARD.png");

	// �U���J�[�h�Ȃ̂ōU���͂�ݒ�
	m_PreDamage = 0;

	// �t�F�[�Y���ݒ�
	m_PheseCount = 3;
}

NormalMoveCard::~NormalMoveCard()
{
}

void NormalMoveCard::Draw(int index, int MaxIdx)
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
		//DrawGraphBloom(LeftTop, LeftTop + XMFLOAT2(width, height), Graph);
	}
	else if (HitCursor) {
		DrawGraph(LeftTop - XMFLOAT2(3, 3), LeftTop + XMFLOAT2(width, height) + XMFLOAT2(3, 3), TexManager::GetColor(ColorConvert2(XMFLOAT4(1, 1, 0, 0.5f))));
	}
}

#pragma region Phese1

void NormalMoveCard::Phese1Start()
{

}

void NormalMoveCard::UpdatePhese1()
{
	// �t�F�[�Y�J�n�����菈��
	if (m_PheseStartTriggerFlag) {
		m_PheseStartTriggerFlag = false;
		Phese1Start();
	}

	// �J�[�h�I��������
	if (Input::isMouseTrigger(M_LEFT)) {
		// �t�F�[�Y���I��
		PheseEndFunc(0);
	}


	
	// �t�F�[�Y�I�������菈��
	if (CanPhaseEnd()) {
		m_WaitTimer = 0;
		m_PheseStartTriggerFlag = false;
		m_isPheseEnd = false;
		Phese1End();
	}
}

void NormalMoveCard::Phese1End()
{
}

#pragma endregion

#pragma region Phese2

void NormalMoveCard::Phese2Start()
{
}

void NormalMoveCard::UpdatePhese2()
{
	// �t�F�[�Y�J�n�����菈��
	if (m_PheseStartTriggerFlag) {
		m_PheseStartTriggerFlag = false;
		Phese2Start();
	}

	if (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip) {
		// �I����x�ڂ̓J�[�h�𔭓����Ȃ�
		//if (m_PreCheckChip == m_NowCheckChip) {
			ChangePlayerDir(pPlayer);

			pPlayer->SetMapPos(m_NowCheckChip);
			GameCamera::Instance()->Targeting(pMap->ChangePos(m_NowCheckChip), GameCamera::Instance()->DEFAULT_FLAME_TIME);

			// �t�F�[�Y���I��
			PheseEndFunc(0);
		//}
	}

	// �t�F�[�Y�I�������菈��
	if (CanPhaseEnd()) {
		m_WaitTimer = 0;
		m_PheseStartTriggerFlag = false;
		m_isPheseEnd = false;
		Phese2End();
	}
}

void NormalMoveCard::Phese2End()
{
}

#pragma endregion

#pragma region Phese3

void NormalMoveCard::Phese3Start()
{
}

void NormalMoveCard::UpdatePhese3()
{
	// �t�F�[�Y�J�n�����菈��
	if (m_PheseStartTriggerFlag) {
		m_PheseStartTriggerFlag = false;
		Phese3Start();
	}

	if (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip) {
		// �I����x�ڂ̓J�[�h�𔭓����Ȃ�
		//if (m_PreCheckChip == m_NowCheckChip) {
			ChangePlayerDir(pPlayer);

			//pPlayer->SetMapPos(m_NowCheckChip);
			//GameCamera::Instance()->Targeting(pMap->ChangePos(m_NowCheckChip), GameCamera::Instance()->DEFAULT_FLAME_TIME);

			// �t�F�[�Y���I��
			PheseEndFunc(0);
		// }
	}

	// �t�F�[�Y�I�������菈��
	if (CanPhaseEnd()) {
		m_WaitTimer = 0;
		m_PheseStartTriggerFlag = false;
		m_isPheseEnd = false;
		Phese3End();
	}
}

void NormalMoveCard::Phese3End()
{
}

#pragma endregion

void NormalMoveCard::SelectTrigger(XMINT2 RefPoint, Cards* pCards)
{
	// �I���\�}�X��ǉ�����
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(1, 0));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-1, 0));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, 1));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, -1));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(2, 0));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-2, 0));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, 2));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(0, -2));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(1, 1));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-1, 1));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(-1, -1));
	pCards->AddCanSelectChips_Move(pPlayer->GetMapPos() + XMINT2(1, -1));
}

void NormalMoveCard::ChangePlayerDir(Player* _pPlayer)
{
	// �U�����������Ɍ�����ύX
	if (m_NowCheckChip.x == pPlayer->GetMapPos().x) {
		if (m_NowCheckChip.y > pPlayer->GetMapPos().y) {
			// ������ݒ�
			pPlayer->SetDir(Charactor::Chara_Dir::BACK);
		}
		else if (m_NowCheckChip.y < pPlayer->GetMapPos().y) {
			// ������ݒ�
			pPlayer->SetDir(Charactor::Chara_Dir::FRONT);
		}
	}
	else {
		if (m_NowCheckChip.x > pPlayer->GetMapPos().x) {
			// ������ݒ�
			pPlayer->SetDir(Charactor::Chara_Dir::RINGHT);
		}
		else if (m_NowCheckChip.x < pPlayer->GetMapPos().x) {
			// ������ݒ�
			pPlayer->SetDir(Charactor::Chara_Dir::LEFT);
		}
	}
}

void NormalMoveCard::SelectTrigger_Dir(XMINT2 RefPoint, Cards* pCards)
{
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, 0));
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, 0));
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, 1));
	pCards->AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, -1));
}

void NormalMoveCard::SetSelectData(XMINT2 _Now, XMINT2 _Pre)
{
	// ���݂̑I���}�X���Z�b�g
	m_NowCheckChip = _Now;
	// �O��̑I���}�X���Z�b�g
	m_PreCheckChip = _Pre;
}
