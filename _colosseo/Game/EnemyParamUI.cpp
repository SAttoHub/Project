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
	StateImg = TexManager::LoadTexture("Resource/image/EnemyStateUI.png");
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

	const XMFLOAT4 WHITE_COL_B = ColorConvert(XMFLOAT4(230.0f, 242.0f, 255.0f, 255.0f));
	const XMFLOAT4 WHITE_COL_L = ColorConvert(XMFLOAT4(0.0f, 0.0f, 0.0f, 255.0f));

	DrawGraph(XMFLOAT2(985.0f, 5.0f), XMFLOAT2(1271.0f, 149.0f), StateImg);
	
	// �G�̖��O
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(1044.0f, 27.0f), 24, WHITE_COL_B, "%s", pEnemy->m_Name.c_str());
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(1031.0f, 62.0f), 28, WHITE_COL_L, "�E%s", pEnemy->m_JobName.c_str());
	
	
	// HP�o�[�`��(�����͉�)

	// �X�^�~�i�`��
	//DrawStrings::Instance()->DrawFormatString(LEFT_POS + XMFLOAT2(0.0f, 32.0f * 3), 32, WHITE, "�X�^�~�i : %d", pEnemy->GetStamina());

	// ���𔖈Â�
	//DrawBox(LEFT_POS, XMFLOAT2(LEFT_POS.x + WIDTH_SIZE, HEIGHT_SIZE), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f));

	// HP�o�[
	// �J�[�h�I�𒆂ł��_���[�W������ꍇ
	int PreDamage = pCards->GetPreDamage();
	if (PreDamage != 0) {
		// �h��͌v�Z��_���[�W�Z�o
		float Damage = float(PreDamage) - float(pEnemy->GetDef());
		Damage < 0 ? Damage = 0 : 0;
		float afterHP = float(pEnemy->GetHP()) - Damage;

		// �v�Z��HP / �ő�HP
		DrawStrings::Instance()->DrawFormatString(XMFLOAT2(1138.0f, 93.0f), 32, WHITE, "%d/%d", int(afterHP), int(pEnemy->GetMaxHP()));
		// HP�o�[��
		//DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f), XMFLOAT2(LEFT_POS.x + WIDTH_SIZE, 64.0f + 32.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
		// ����������Ԃ��`��
		/*DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f),
			XMFLOAT2(LEFT_POS.x + WIDTH_SIZE * (float(pEnemy->GetHP()) / float(pEnemy->GetMaxHP())), 64.0f + 32.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));*/
		XMFLOAT2 LT = XMFLOAT2(1011.0f, 124.0f);
		XMFLOAT2 RB = XMFLOAT2(1243.0f, 129.0f);
		float width = RB.x - LT.x;
		// ����HP / �ő�HP
		DrawGraph(LT,
			XMFLOAT2(LT.x + width * (float(pEnemy->GetHP()) / float(pEnemy->GetMaxHP())), RB.y),
			TexManager::GetColor(XMFLOAT4(2255.0f, 0.0f, 0.0f, 255.0f)));
		if (afterHP > 0) {
			// �c���΂ŕ`��
			/*DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f),
				XMFLOAT2(LEFT_POS.x + WIDTH_SIZE * (afterHP / float(pEnemy->GetMaxHP())), 64.0f + 32.0f),
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));*/
			DrawGraph(LT,
				XMFLOAT2(LT.x + width * (afterHP / float(pEnemy->GetMaxHP())), RB.y),
				TexManager::GetColor(XMFLOAT4(230.0f, 242.0f, 255.0f, 255.0f)));
		}
	}
	else {
		// ����HP / �ő�HP
		//DrawStrings::Instance()->DrawFormatString(LEFT_POS + XMFLOAT2(0.0f, 32.0f), 32, WHITE, "%d / %d", int(pEnemy->GetHP()), int(pEnemy->GetMaxHP()));
		DrawStrings::Instance()->DrawFormatString(XMFLOAT2(1138.0f, 93.0f), 32, WHITE, "%d/%d", int(pEnemy->GetHP()), int(pEnemy->GetMaxHP()));
		/*DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f), XMFLOAT2(LEFT_POS.x + WIDTH_SIZE, 64.0f + 32.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
		DrawBox(LEFT_POS + XMFLOAT2(0.0f, 64.0f),
			XMFLOAT2(LEFT_POS.x + WIDTH_SIZE * (float(pEnemy->GetHP()) / float(pEnemy->GetMaxHP())), 64.0f + 32.0f),
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));*/

		XMFLOAT2 LT = XMFLOAT2(1011.0f, 124.0f);
		XMFLOAT2 RB = XMFLOAT2(1243.0f, 129.0f);
		float width = RB.x - LT.x;
		// ����HP / �ő�HP
		DrawGraph(LT,
			XMFLOAT2(LT.x + width * (float(pEnemy->GetHP()) / float(pEnemy->GetMaxHP())), RB.y),
			TexManager::GetColor(XMFLOAT4(230.0f, 242.0f, 255.0f, 255.0f)));
	}
}
