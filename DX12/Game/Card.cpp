#include "Card.h"

Card::Card(CardType Type)
{
	width = 100.0f;
	height = 200.0f;
	LeftTop = {};
	HitCursor = false;
	m_Type = Type;
	Active = false;

	switch (m_Type)
	{
	case CardType::NONE:
		Graph = TexManager::LoadTexture("Resource/image/kougeki.png");
		break;
	case CardType::DEFAULT_ATTACK:
		Graph = TexManager::LoadTexture("Resource/image/kougeki.png");
		break;
	case CardType::DEFAULT_MOVE:
		Graph = TexManager::LoadTexture("Resource/image/idou.png");
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
	}
	/*if (col) {
		delete col;
	}*/
}

void Card::SetCollider(GameBaseCollider *collider)
{
	collider->SetBasisPosition(&LeftTop);
	this->collider = collider;
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
	LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f, WINDOW_HEIGHT - 200.0f };
	ColliderUpdate();
}

void Card::Draw(int index, int MaxIdx)
{
	float width = 100.0f;
	float height = 200.0f;
	//XMFLOAT2 LeftTop;
	float CenterIdx = MaxIdx / 2.0f;
	//if (MaxIdx % 2 == 0) {
		LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f, WINDOW_HEIGHT - 200.0f + 150.0f };
	/*}
	if (MaxIdx % 2 != 0) {
		LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f, WINDOW_HEIGHT - 200.0f };
	}*/
	// Œã‚Åƒ^ƒCƒv‚²‚Æ‚É•Ê‰æ‘œ‚ð•`‰æ‚·‚é‚æ‚¤‚É‚·‚é
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

void Card::Initialize(int index, int MaxIdx)
{
	float CenterIdx = MaxIdx / 2.0f;
	LeftTop = { WINDOW_WIDTH / 2.0f + (float(index) - CenterIdx) * width - width / 2.0f, WINDOW_HEIGHT - 200.0f };
	col = new GameBoxCollider(&LeftTop, XMFLOAT2(0, 0), XMFLOAT2(width, height), "Card");
	SetCollider(col);
}
