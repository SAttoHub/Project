#include "NormalAttackCard.h"
#include "../Input.h"
#include "Player.h"
#include "Enemys.h"
#include "Cards.h"

NormalAttackCard::NormalAttackCard()
{
	ConstructInitialize();
	m_Type = CardType::DEFAULT_ATTACK;
	Graph = TexManager::LoadTexture("Resource/image/ATK_CARD.png");

	// �U���J�[�h�Ȃ̂ōU���͂�ݒ�
	m_PreDamage = 1;

	// �t�F�[�Y���ݒ�
	m_PheseCount = 2;
}

NormalAttackCard::~NormalAttackCard()
{
}

void NormalAttackCard::Draw(int index, int MaxIdx)
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

void NormalAttackCard::Phese1Start()
{
	
}

void NormalAttackCard::UpdatePhese1()
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

void NormalAttackCard::Phese1End()
{
}

#pragma endregion

#pragma region Phese2

void NormalAttackCard::Phese2Start()
{
}

void NormalAttackCard::UpdatePhese2()
{
	// �t�F�[�Y�J�n�����菈��
	if (m_PheseStartTriggerFlag) {
		m_PheseStartTriggerFlag = false;
		Phese2Start();
	}
	if (Input::isMouseTrigger(M_LEFT) && pMap->isHitChip) {
		// �I����x�ڂ̓J�[�h�𔭓����Ȃ�
		if (m_PreCheckChip == m_NowCheckChip) {
			// �G
			Enemy* en = pEnemys->GetEnemy(m_NowCheckChip);
			if (en == nullptr) {
				return;
			}

			ChangePlayerDir(pPlayer);

			en->Damage(m_PreDamage);
			en->DamageReaction(pPlayer->GetMapPos(), en->GetMapPos());

			// �t�F�[�Y���I��
			PheseEndFunc(0);
		}
	}
	// �t�F�[�Y�I�������菈��
	if (CanPhaseEnd()) {
		m_WaitTimer = 0;
		m_PheseStartTriggerFlag = false;
		m_isPheseEnd = false;
		Phese2End();
	}
}

void NormalAttackCard::Phese2End()
{
}

#pragma endregion

#pragma region Phese3

void NormalAttackCard::Phese3Start()
{
}

void NormalAttackCard::UpdatePhese3()
{
}

void NormalAttackCard::Phese3End()
{
}

#pragma endregion

void NormalAttackCard::SelectTrigger(XMINT2 RefPoint, Cards* pCards)
{
	// �I���\�}�X��ǉ�����
	pCards->AddCanSelectChips(RefPoint + XMINT2(1, 0));
	pCards->AddCanSelectChips(RefPoint + XMINT2(-1, 0));
	pCards->AddCanSelectChips(RefPoint + XMINT2(0, 1));
	pCards->AddCanSelectChips(RefPoint + XMINT2(0, -1));
}

void NormalAttackCard::ChangePlayerDir(Player* _pPlayer)
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

void NormalAttackCard::SetSelectData(XMINT2 _Now, XMINT2 _Pre)
{
	// ���݂̑I���}�X���Z�b�g
	m_NowCheckChip = _Now;
	// �O��̑I���}�X���Z�b�g
	m_PreCheckChip = _Pre;
}
