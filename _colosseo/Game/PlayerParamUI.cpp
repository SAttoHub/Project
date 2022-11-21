#include "PlayerParamUI.h"
#include "Enemy.h"
#include "Player.h"
#include "Cards.h"
#include "..\TexManager.h"

PlayerParamUI::PlayerParamUI()
{
	//pEnemy = nullptr;
	pPlayer = nullptr;
	pCards = nullptr;
	m_RefPoint = XMFLOAT2(0.0f, 0.0f);
}

PlayerParamUI::~PlayerParamUI()
{
}

//void PlayerParamUI::SetEnemyPtr(Enemy* enemy)
//{
//	pEnemy = nullptr;
//	pEnemy = enemy;
//}

void PlayerParamUI::SetPlayerPtr(Player* player)
{
	pPlayer = player;
	Img = TexManager::LoadTexture("Resource/image/Chara/Chara1.png");
}

void PlayerParamUI::SetCardsPtr(Cards* cards)
{
	pCards = cards;
}

void PlayerParamUI::Update()
{
}

void PlayerParamUI::Draw()
{
	if (pPlayer == nullptr) {
		return;
	}
	// ��
	const XMFLOAT4 ZeldaWHITE = ColorConvert(Color_Zelda_White);
	// UI�E�B���h�E������W
	const XMFLOAT2 LEFT_POS = m_RefPoint;
	// UI�E�B���h�E����
	const float WIDTH_SIZE = 200.0f;
	// UI�E�B���h�E�c��
	float HEIGHT_SIZE = 200.0f;

	// �G�̖��O
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(165.0f, 570.0f), 32, ZeldaWHITE, "Player");

	// HP�o�[�`��(�����͉�)

	// �X�^�~�i�`��
	//DrawStrings::Instance()->DrawFormatString(LEFT_POS + XMFLOAT2(0.0f, 32.0f * 3), 32, WHITE, "�X�^�~�i : %d", pEnemy->GetStamina());

	// ���𔖈Â�
	DrawBox(XMFLOAT2(0.0f, 480.0f), XMFLOAT2(149.0f, 720.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.35f));
	DrawBox(XMFLOAT2(149.0f, 566.0f), XMFLOAT2(437.0f, 710.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.35f));

	DrawGraph(XMFLOAT2(0.0f, 480.0f), XMFLOAT2(149.0f, 720.0f), Img);

	// HP�o�[
	DrawBox(XMFLOAT2(159.0f, 664.0f), XMFLOAT2(428.0f, 703.0f), ZeldaWHITE);
	DrawBox(XMFLOAT2(162.0f, 667.0f), XMFLOAT2(425.0f, 700.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
	XMFLOAT2 LT = XMFLOAT2(164.0f, 669.0f);
	XMFLOAT2 RB = XMFLOAT2(423.0f, 698.0f);
	float width = RB.x - LT.x;
	// ����HP / �ő�HP
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(283.0f, 630.0f), 32, ZeldaWHITE, "%d / %d", pPlayer->GetHP(), pPlayer->GetMaxHP());
	DrawBox(LT,
		XMFLOAT2(LT.x + width * (float(pPlayer->GetHP()) / float(pPlayer->GetMaxHP())), RB.y),
		ZeldaWHITE);
}
