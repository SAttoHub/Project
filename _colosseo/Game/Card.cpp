#include "Card.h"
#include "Player.h"
#include "Enemys.h"
#include "Map.h"

Card::Card()
{
	ConstructInitialize();

	switch (m_Type)
	{
	case CardType::NONE:
		Graph = TexManager::LoadTexture("Resource/image/ATK_CARD.png");
		break;
	case CardType::DEFAULT_ATTACK:
		Graph = TexManager::LoadTexture("Resource/image/ATK_CARD.png");
		break;
	case CardType::DEFAULT_MOVE:
		Graph = TexManager::LoadTexture("Resource/image/MOVE_CARD.png");
		break;
	default:
		break;
	}
}

Card::~Card()
{
	if (collider) {
		GCM::Instance()->RemoveCollider(collider);
		//delete collider;
		//delete col;
	}
	/*if (col) {
		delete col;
	}*/
}

void Card::SetCollider()
{
	collider = new GameBoxCollider(&LeftTop, XMFLOAT2(0, 0), XMFLOAT2(width, height), "Card");
	collider->SetBasisPosition(&LeftTop);
	GCM::Instance()->AddCollider(collider);
}

void Card::ColliderUpdate()
{
	if (collider) {
		collider->Update();
	}
}

void Card::Update(int index, int MaxIdx)
{
	float CenterIdx = MaxIdx / 2.0f;
	LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f + 100.0f, WINDOW_HEIGHT - 200.0f };
	ColliderUpdate();
}

void Card::Draw(int index, int MaxIdx)
{
	float width = 100.0f;
	float height = 200.0f;
	//XMFLOAT2 LeftTop;
	float CenterIdx = MaxIdx / 2.0f;
	//if (MaxIdx % 2 == 0) {
		LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f + 100.0f, WINDOW_HEIGHT - 200.0f/* + 150.0f */};
	/*}
	if (MaxIdx % 2 != 0) {
		LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f, WINDOW_HEIGHT - 200.0f };
	}*/
	// 後でタイプごとに別画像を描画するようにする
	switch (m_Type)
	{
	case CardType::NONE:
		//DrawBox(LeftTop, LeftTop + XMFLOAT2(width, height), XMFLOAT4(0.8f, 0.8f, 0.8f, 1));
		DrawGraph(LeftTop, LeftTop + XMFLOAT2(width, height), Graph);
		break;
	case CardType::DEFAULT_ATTACK:
		//DrawBox(LeftTop, LeftTop + XMFLOAT2(width, height), XMFLOAT4(1, 0, 0, 1));
		DrawGraph(LeftTop, LeftTop + XMFLOAT2(width, height), Graph);
		break;
	case CardType::DEFAULT_MOVE:
		//DrawBox(LeftTop, LeftTop + XMFLOAT2(width, height), XMFLOAT4(0, 1, 0, 1));
		DrawGraph(LeftTop, LeftTop + XMFLOAT2(width, height), Graph);
		break;
	default:
		break;
	}
	if (Active) {
		//DrawBox(LeftTop - XMFLOAT2(3, 3), LeftTop + XMFLOAT2(width, height) + XMFLOAT2(3, 3), XMFLOAT4(1, 0, 1, 0.5f));
		DrawGraph(LeftTop - XMFLOAT2(3, 3), LeftTop + XMFLOAT2(width, height) + XMFLOAT2(3, 3), TexManager::GetColor(ColorConvert2(XMFLOAT4(1, 0, 1, 0.5f))));
	}
	else if (HitCursor) {
		//DrawBox(LeftTop - XMFLOAT2(3, 3), LeftTop + XMFLOAT2(width, height) + XMFLOAT2(3, 3), XMFLOAT4(1, 1, 0, 0.5f));
		DrawGraph(LeftTop - XMFLOAT2(3, 3), LeftTop + XMFLOAT2(width, height) + XMFLOAT2(3, 3), TexManager::GetColor(ColorConvert2(XMFLOAT4(1, 1, 0, 0.5f))));
	}
}

void Card::ConstructInitialize()
{
	width = 100.0f;
	height = 200.0f;
	LeftTop = {};
	HitCursor = false;
	Active = false;
}

void Card::SetPlEnPtr(Player* _pPlayer, Enemys* _pEnemys, Map* _pMap)
{
	pPlayer = _pPlayer;
	pEnemys = _pEnemys;
	pMap = _pMap;
}

bool Card::CanPhaseEnd()
{
	// WaitTimerが0より大きい時にm_WaitTimerを減少させる
	m_WaitTimer < 0 ? m_WaitTimer = 0 : m_WaitTimer--;
	
	// ターンが終了できる場合
	if (m_isPheseEnd && m_WaitTimer <= 0) {
		m_PheseChange = true;
		return true;
	}

	return false;
}

void Card::PheseEndFunc(int _WaitFlame)
{
	// フェーズ終了フラグを立てる
	m_isPheseEnd = true;
	// 待機フレームを設定する
	m_WaitTimer = _WaitFlame;
}



void Card::Initialize(int index, int MaxIdx)
{
	float CenterIdx = MaxIdx / 2.0f;
	LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f + 100.0f, WINDOW_HEIGHT - 200.0f };
	SetCollider();
}
