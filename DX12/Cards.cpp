#include "Cards.h"
#include "Input.h"

Cards::Cards()
{
	//m_Cards.reserve(16);
	m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	//m_Cards.push_front(Card(CardType::NONE));

	m_UseCardType = CardType::NONE;
	NowPhase = 0;
	TurnEnd = false;

	TurnEndButton = TexManager::LoadTexture("Resource/image/TurnEnd.png");

	LT = XMFLOAT2(WINDOW_WIDTH - 300, WINDOW_HEIGHT - 300);

	isMyTurn = false;
}

Cards::~Cards()
{

}

void Cards::Initialize(Player *_Player, Enemys *_Enemys, Map *_Map)
{
	pPlayer = _Player;
	pEnemys = _Enemys;
	pMap = _Map;
	for (int i = 0; i < m_Cards.size(); i++) {
		m_Cards[i]->Initialize(i, m_Cards.size());
	}
}

void Cards::Update()
{
	for (int i = 0; i < m_Cards.size(); i++) {
		m_Cards[i]->Update(i, m_Cards.size());
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
	for (int i = 0; i < m_Cards.size(); i++) {
		m_Cards[i]->Draw(i, m_Cards.size());
	}

	if (m_UseCardType != CardType::NONE) {
		DrawCube(XMFLOAT3(float(m_NowSelectChip.x) * ChipData::CHIP_SIZE, 0, float(m_NowSelectChip.y) * ChipData::CHIP_SIZE),
			XMFLOAT3(float(m_NowSelectChip.x) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE, 5.1f, float(m_NowSelectChip.y) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE),
			ColorConvert(255, 255, 0, 255));

		for (auto &data : CanSelectChips) {
			DrawCube(XMFLOAT3(float(data.x) * ChipData::CHIP_SIZE, 0, float(data.y) * ChipData::CHIP_SIZE),
				XMFLOAT3(float(data.x) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE, 2.1f, float(data.y) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE),
				ColorConvert(0, 255, 0, 150));
		}

	}
	else if(isMyTurn) {
		DrawGraph(LT,
			LT + XMFLOAT2(160, 64) * 1.5f,
			TurnEndButton);
		XMFLOAT2 point = Input::GetMousePos();

		if (point.x >= LT.x && point.x <= LT.x + 160 * 1.5f &&
			point.y >= LT.y && point.y <= LT.y + 64 * 1.5f) {
			DrawGraph(LT - XMFLOAT2(2, 2),
				LT + XMFLOAT2(160, 64) * 1.5f + XMFLOAT2(2, 2),
				TexManager::GetColor(ColorConvert2(XMFLOAT4(1, 1, 0, 0.5f))));
		}
	}
}

void Cards::StartTurn()
{
	m_Cards.clear();
	m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_ATTACK));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));
	m_Cards.push_front(Card(CardType::DEFAULT_MOVE));

	m_UseCardType = CardType::NONE;
	NowPhase = 0;
	for (int i = 0; i < m_Cards.size(); i++) {
		m_Cards[i]->Initialize(i, m_Cards.size());
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

void Cards::PlayerTurnUpdate()
{
	if (NowPhase == 0) {
		CardPhaseUpdate();
	}
	else if (NowPhase == 1) {
		CardEffectPhaseUpdate();
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
					NowPhase = 1;

					if (m_UseCardType == CardType::DEFAULT_ATTACK) {
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, 1));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, -1));
					}
					else if (m_UseCardType == CardType::DEFAULT_MOVE) {
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, 1));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, -1));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(2, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-2, 0));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, 2));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(0, -2));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, 1));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, 1));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(-1, -1));
						AddCanSelectChips(pPlayer->GetMapPos() + XMINT2(1, -1));

						for (int x = 0; x < CanSelectChips.size(); x++){
							if (pEnemys->GetEnemy(CanSelectChips[x]) != nullptr) {
								CanSelectChips.erase(CanSelectChips.begin() + x);
							}
						}
					}

					break;
				}
			}
		}
	}

	XMFLOAT2 point = Input::GetMousePos();
	if (point.x >= LT.x && point.x <= LT.x + 200 &&
		point.y >= LT.y && point.y <= LT.y + 200) {
		if (Input::isMouseTrigger(M_LEFT)) {
			TurnEnd = true;
			isMyTurn = false;
		}
	}
	/*if (Input::isKeyTrigger(DIK_RETURN)) {
		TurnEnd = true;
	}*/
}

void Cards::CardEffectPhaseUpdate()
{
	if (Input::isKeyTrigger(DIK_BACKSPACE)) {
		m_UseCardType = CardType::NONE;
		m_Cards[m_UseCardIndex]->Active = false;
		NowPhase = 0;
		CanSelectChips.clear();
	}

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

	if (m_UseCardType == CardType::DEFAULT_ATTACK) {
		if (Input::isKeyTrigger(DIK_RETURN)) {
			Enemy *en = pEnemys->GetEnemy(m_NowSelectChip);
			if (en != nullptr && ExistInCanSelectChips(m_NowSelectChip)) {
				en->Damage(1);

				m_Cards.erase_after(std::next(m_Cards.before_begin(), m_UseCardIndex));
				m_UseCardType = CardType::NONE;
				NowPhase = 0;
				CanSelectChips.clear();
			}
		}
	}
	else if (m_UseCardType == CardType::DEFAULT_MOVE) {
		if (Input::isKeyTrigger(DIK_RETURN)) {
			if (pMap->InMap(m_NowSelectChip) && ExistInCanSelectChips(m_NowSelectChip)) {
				m_Cards.erase_after(std::next(m_Cards.before_begin(), m_UseCardIndex));
				pPlayer->SetMapPos(m_NowSelectChip);
				m_UseCardType = CardType::NONE;
				NowPhase = 0;
				CanSelectChips.clear();
			}
		}
	}
}
