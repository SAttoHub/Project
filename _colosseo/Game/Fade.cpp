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
	NextWaveFade = TexManager::LoadTexture("Resource/image/NextWave.png");
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

void Fade::DrawWaveFade(int Flame, int MaxFlame)
{
	float F1 = float(MaxFlame) / 3.0f;

	if (Flame < F1) {
		/*DrawGraph(XMFLOAT2(0 - float(Ease::OutExpo((float)Flame / F1)) * float(WINDOW_WIDTH), 0),
			XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo((float)Flame / F1)) * float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
			NextWaveFade);*/
		DrawGraph(XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo((float)Flame / F1)) * float(WINDOW_WIDTH), 0),
			XMFLOAT2(float(WINDOW_WIDTH + WINDOW_WIDTH) - float(Ease::OutExpo((float)Flame / F1)) * float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
			NextWaveFade);
	}
	else if (Flame < (F1 * 2)) {
		DrawGraph(XMFLOAT2(0, 0),
			XMFLOAT2(float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
			NextWaveFade);
	}
	else if (Flame < MaxFlame) {
		/*DrawGraph(XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo(((float)Flame - (F1 * 2.0f)) / ((float)MaxFlame - (F1 * 2.0f)))) * float(WINDOW_WIDTH), 0),
			XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo(((float)Flame - (F1 * 2.0f)) / ((float)MaxFlame - (F1 * 2.0f)))) * float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
			NextWaveFade);*/
		/*DrawGraph(XMFLOAT2(0 - float(Ease::OutExpo((float)Flame / (F1 * 2.0f))) * float(WINDOW_WIDTH), 0),
			XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo((float)Flame / F1)) * float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
			NextWaveFade);*/

		DrawGraph(XMFLOAT2(-float(Ease::OutExpo(((float)Flame - (F1 * 2.0f)) / ((float)MaxFlame - (F1 * 2.0f)))) * float(WINDOW_WIDTH), 0),
			XMFLOAT2(float(WINDOW_WIDTH) - float(Ease::OutExpo(((float)Flame - (F1 * 2.0f)) / ((float)MaxFlame - (F1 * 2.0f)))) * float(WINDOW_WIDTH), float(WINDOW_HEIGHT)),
			NextWaveFade);
	}
}
