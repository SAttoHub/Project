#include "Fade.h"
#include "../Engine/Primitive/PrimitiveFunc.h"
#include "../TexManager.h"

Fade::Fade()
{
	LoadFadeResouces();
}

Fade::~Fade()
{
}

void Fade::LoadFadeResouces()
{
	NormalFade = TexManager::GetColor(XMFLOAT4(10, 10, 10, 255));
}

void Fade::DrawNormalFade(int Flame, int MaxFlame)
{
	DrawGraph(XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo((float)Flame / (float)MaxFlame)) * float(WINDOW_WIDTH), 0),
		XMFLOAT2(float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
		NormalFade);
}

void Fade::DrawNormalFadeIn(int Flame, int MaxFlame)
{
	DrawGraph(XMFLOAT2(0, 0),
		XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo((float)Flame / (float)MaxFlame)) * float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
		NormalFade);
}
