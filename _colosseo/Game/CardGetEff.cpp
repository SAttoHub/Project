#include "CardGetEff.h"

CardGetEff::CardGetEff(CardType _GetCardType)
{
	m_Timer = 0;
	m_isEnd = false;
	m_CardType = _GetCardType;

	m_CenterPos = XMFLOAT2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
	m_CardSize = XMFLOAT2(100.0f, 200.0f);
	m_Size = 0.0f;
}

CardGetEff::~CardGetEff()
{
}

void CardGetEff::Update()
{
	// ŽžŠÔ‚ð‰ÁŽZ‚·‚é
	if (MAX_TIME > m_Timer) {
		m_Timer++;
	}
	m_Size = Ease::EaseFunc(EaseName::OutBack, 0.0f, 1.0f, m_Timer, MAX_TIME);

	if (m_Timer >= MAX_TIME && Input::isMouseTrigger(M_LEFT)) {
		m_isEnd = true;
	}
}

void CardGetEff::Draw()
{
	// Œã‚ë‚ðˆÃ‚­
	DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT),
		TexManager::GetColor(ColorConvert2(XMFLOAT4(0.1f, 0.1f, 0.1f, 0.3f))));
	// ƒJ[ƒh‚ð•`‰æ
	DrawGraph(m_CenterPos - (m_CardSize * m_Size), m_CenterPos + (m_CardSize * m_Size),
		Cards::CardTexture[int(m_CardType)]);

	if (m_Timer >= MAX_TIME) {
		// F
		const XMFLOAT4 WHITE_COL_B = ColorConvert(XMFLOAT4(230.0f, 242.0f, 255.0f, 255.0f));

		DrawStrings::Instance()->DrawFormatString(XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT) - XMFLOAT2(300.0f, 64.0f),
			64.0f, WHITE_COL_B, "LEFT CLECK ->");
	}
}
