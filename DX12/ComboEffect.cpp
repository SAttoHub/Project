#include "ComboEffect.h"
#include "ease.h"
#include "Parameters.h"
ComboEffect::ComboEffect(XMFLOAT2 p1, int EndTime, int Combo)
{
	Pos = p1;
	this->EndTime = EndTime;
	this->EndTime = 80;
	Timer = 0;
	alpha = 255;
	this->Combo = Combo;
	P = Pos.y;
}

ComboEffect::~ComboEffect()
{
}

void ComboEffect::Initialize()
{
	Timer = 0;
}

void ComboEffect::Update()
{
	Timer++;
	//alpha = 255 * ((float)Timer / (float)EndTime);
	//Pos.y -= 0.2f;

	scale = Ease::EaseFunc(EaseName::OutBack, 0, 1, Timer, 80);
	
	/*if (Timer < 60) {
		scale = Ease::EaseFunc(EaseName::OutExpo, 0, 1, Timer, 60);
	}*/

	if (Timer > 30) {
		Pos.y = P - Ease::EaseFunc(EaseName::InCubic, 0, 10, Timer - 30, EndTime - 30);
	}
	if (Timer > 80) {
		alpha = 255 * Ease::EaseFunc(EaseName::InCubic, 1, 0, Timer - 80, EndTime - 80);
	}
}

void ComboEffect::Draw()
{
	/*DrawBox(XMFLOAT2(Pos.x - 36, Pos.y - 8) - 3, XMFLOAT2(Pos.x - 36 + 16 * scale * 4, Pos.y - 8 + 16 * scale) + 3
		, ColorConvert(10, 10, 10, 240 * (alpha / 255.0f)));
	DrawBox(XMFLOAT2(Pos.x - 36, Pos.y - 8), XMFLOAT2(Pos.x - 36 + 16 * scale * 4, Pos.y - 8 + 16 * scale)
		, ColorConvert(30, 30, 30, 190 * (alpha / 255.0f)));
	DrawStrings::Instance()->DrawFormatString(
		XMFLOAT2(Pos.x - 36, Pos.y - 8),
		16 * scale,
		XMFLOAT4(ColorConvert(230, 230, 230, alpha)),
		"%dCombo!", Combo
	);*/
	float tate = 37;
	float yoko = 74;
	XMFLOAT2 P = XMFLOAT2(Pos.x, Pos.y);
	if (Combo < 30) {
		int c = Combo;
		if (c < 1) {
			c = 1;
		}
		DrawGraphEffColor(P - (XMFLOAT2(yoko, tate) / 2.0f * scale), P + (XMFLOAT2(yoko, tate) / 2.0f * scale),
			Parameters::Instance()->ComboNumTex[c - 1],
			XMFLOAT4(1, 1, 1, 0.7f * (alpha / 255.0f)));
	}
	else {
		DrawGraphEffColor(P - (XMFLOAT2(yoko, tate) / 2.0f * scale), P + (XMFLOAT2(yoko, tate) / 2.0f * scale),
			Parameters::Instance()->ComboNumTex[29],
			XMFLOAT4(1, 1, 1, 0.7f * (alpha / 255.0f)));
	}
}

void ComboEffect::Finalize()
{
}
