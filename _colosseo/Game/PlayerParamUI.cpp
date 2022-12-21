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
	StateImg = TexManager::LoadTexture("Resource/image/PlayerStateUI.png");
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

	DrawGraph(XMFLOAT2(0.0f, 473.0f), XMFLOAT2(435.0f, 720.0f), StateImg);

	// 白
	//const XMFLOAT4 ZeldaWHITE = ColorConvert(Color_Zelda_White);
	// UIウィンドウ左上座標
	//const XMFLOAT2 LEFT_POS = m_RefPoint;
	// UIウィンドウ横幅
	//const float WIDTH_SIZE = 200.0f;
	// UIウィンドウ縦幅
	//float HEIGHT_SIZE = 200.0f;

	const XMFLOAT4 WHITE_COL_B = ColorConvert(XMFLOAT4(230.0f, 242.0f, 255.0f ,255.0f));
	const XMFLOAT4 WHITE_COL_L = ColorConvert(XMFLOAT4(0.0f, 0.0f, 0.0f, 255.0f));

	// 敵の名前
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(208.0f, 588.0f), 24, WHITE_COL_B, "Player");

	// HPバー描画(処理は下)

	// スタミナ描画
	//DrawStrings::Instance()->DrawFormatString(LEFT_POS + XMFLOAT2(0.0f, 32.0f * 3), 32, WHITE, "スタミナ : %d", pEnemy->GetStamina());

	// 後ろを薄暗く
	//DrawBox(XMFLOAT2(0.0f, 480.0f), XMFLOAT2(149.0f, 720.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.35f));
	//DrawBox(XMFLOAT2(149.0f, 566.0f), XMFLOAT2(437.0f, 710.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.35f));

	//DrawGraph(XMFLOAT2(0.0f, 480.0f), XMFLOAT2(149.0f, 720.0f), Img);

	// HPバー
	//DrawBox(XMFLOAT2(159.0f, 664.0f), XMFLOAT2(428.0f, 703.0f), WHITE_COL_B);
	//DrawBox(XMFLOAT2(162.0f, 667.0f), XMFLOAT2(425.0f, 700.0f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(193.0f, 623.0f), 28, WHITE_COL_L, "・剣士");
	XMFLOAT2 LT = XMFLOAT2(175.0f, 685.0f);
	XMFLOAT2 RB = XMFLOAT2(407.0f, 690.0f);
	float width = RB.x - LT.x;
	// 現在HP / 最大HP
	DrawStrings::Instance()->DrawFormatString(XMFLOAT2(300.0f, 654.0f), 32, WHITE_COL_B, "%d/%d", pPlayer->GetHP(), pPlayer->GetMaxHP());
	DrawGraph(LT,
		XMFLOAT2(LT.x + width * (float(pPlayer->GetHP()) / float(pPlayer->GetMaxHP())), RB.y),
		TexManager::GetColor(XMFLOAT4(230.0f, 242.0f, 255.0f, 255.0f)));
}
