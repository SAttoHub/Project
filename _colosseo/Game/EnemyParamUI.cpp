#include "EnemyParamUI.h"
#include "Enemy.h"
#include "Player.h"
#include "Cards.h"

EnemyParamUI::EnemyParamUI()
{
	pEnemy = nullptr;
	pPlayer = nullptr;
	m_RefPoint = XMFLOAT2(0.0f, 0.0f);
}

EnemyParamUI::~EnemyParamUI()
{
}

void EnemyParamUI::SetEnemyPtr(Enemy* enemy)
{
	pEnemy = nullptr;
	pEnemy = enemy;
}

void EnemyParamUI::SetPlayerPtr(Player* player)
{
	pPlayer = player;
}

void EnemyParamUI::SetCardsPtr(Cards* cards)
{
	pCards = cards;
}

void EnemyParamUI::Update()
{
}

void EnemyParamUI::Draw()
{
	if (pEnemy == nullptr) {
		return;
	}
	// ��
	const XMFLOAT4 WHITE = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// UI�E�B���h�E������W
	const XMFLOAT2 LEFT_POS = m_RefPoint;
	// UI�E�B���h�E����
	const float WIDTH_SIZE = 200.0f;
	// UI�E�B���h�E�c��
	float HEIGHT_SIZE = 200.0f;
	
	// �G�̖��O
	DrawStrings::Instance()->DrawFormatString(LEFT_POS, 32, WHITE, "%s", pEnemy->m_Name.c_str());
	
	// HP�o�[�`��(�����͉�)

	// �X�^�~�i�`��
	DrawStrings::Instance()->DrawFormatString(LEFT_POS + XMFLOAT2(0.0f, 32.0f * 3), 32, WHITE, "�X�^�~�i : %d", pEnemy->GetStamina());

	// ���𔖈Â�
	DrawBox(LEFT_POS, XMFLOAT2(LEFT_POS.x + WIDTH_SIZE, HEIGHT_SIZE), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f));

	// HP�o�[
	// �J�[�h�I�𒆂ł��_���[�W������ꍇ
	int PreDamage = pCards->GetPreDamage();
	if (PreDamage != 0) {
		// �h��͌v�Z��_���[�W�Z�o
		float Damage = float(PreDamage) - float(pEnemy->GetDef());
		Damage < 0 ? Damage = 0 : 0;
		float afterHP = float(pEnemy->GetHP()) - Damage;

		// �v�Z��HP / �ő�HP
		DrawStrings::Instance()->DrawFormatString(LEFT_POS + XMFLOAT2(0.0f, 32.0f), 32, WHITE, "%d / %d", int(afterHP), int(pEnemy->GetMaxHP()));
		// HP�o�[��
		DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f), XMFLOAT2(LEFT_POS.x + WIDTH_SIZE, 64.0f + 32.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
		// ����������Ԃ��`��
		DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f),
			XMFLOAT2(LEFT_POS.x + WIDTH_SIZE * (float(pEnemy->GetHP()) / float(pEnemy->GetMaxHP())), 64.0f + 32.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		if (afterHP > 0) {
			// �c���΂ŕ`��
			DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f),
				XMFLOAT2(LEFT_POS.x + WIDTH_SIZE * (afterHP / float(pEnemy->GetMaxHP())), 64.0f + 32.0f),
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}
	else {
		// ����HP / �ő�HP
		DrawStrings::Instance()->DrawFormatString(LEFT_POS + XMFLOAT2(0.0f, 32.0f), 32, WHITE, "%d / %d", int(pEnemy->GetHP()), int(pEnemy->GetMaxHP()));
		DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f), XMFLOAT2(LEFT_POS.x + WIDTH_SIZE, 64.0f + 32.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
		DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f),
			XMFLOAT2(LEFT_POS.x + WIDTH_SIZE * (float(pEnemy->GetHP()) / float(pEnemy->GetMaxHP())), 64.0f + 32.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	}
}
