#include "NumsDraw.h"
#include "TexManager.h"
#include "Engine\Primitive\PrimitiveFunc.h"
#include "Smath.h"

void NumsDraw::LoadNumderTexture()
{
	/*TextureNumber[0] = TexManager::LoadTexture("Resource/image/moji/moji_0.png");
	TextureNumber[1] = TexManager::LoadTexture("Resource/image/moji/moji_1.png");
	TextureNumber[2] = TexManager::LoadTexture("Resource/image/moji/moji_2.png");
	TextureNumber[3] = TexManager::LoadTexture("Resource/image/moji/moji_3.png");
	TextureNumber[4] = TexManager::LoadTexture("Resource/image/moji/moji_4.png");
	TextureNumber[5] = TexManager::LoadTexture("Resource/image/moji/moji_5.png");
	TextureNumber[6] = TexManager::LoadTexture("Resource/image/moji/moji_6.png");
	TextureNumber[7] = TexManager::LoadTexture("Resource/image/moji/moji_7.png");
	TextureNumber[8] = TexManager::LoadTexture("Resource/image/moji/moji_8.png");
	TextureNumber[9] = TexManager::LoadTexture("Resource/image/moji/moji_9.png");*/
}

NumsDraw::NumsDraw()
{
	//LoadNumderTexture();
}

void NumsDraw::DrawNumber(XMFLOAT2 pos1, XMFLOAT2 pos2, int Number, XMFLOAT4 color)
{
	//int digit = (int)(Number / 10) + 1;
	//int digit = log10(Number) + 1;
	//float width = pos2.x - pos1.x;
	//float w = width / (float)digit;

	//for (int i = 0; i < digit; i++) {
	//	/*DrawGraph(XMFLOAT2(pos1.x + width * i, pos1.y),
	//		XMFLOAT2(pos1.x + width * (i + 1), pos2.y),
	//		TextureNumber[Smath::GetDigits(Number, i, i)]);*/
	//	DrawGraphEffColor(XMFLOAT2(pos1.x + w * i, pos1.y),
	//		XMFLOAT2(pos1.x + w * (i + 1), pos2.y),
	//		TextureNumber[Smath::GetDigits(Number, i, i)], color);
	//}

	/*DrawGraph(XMFLOAT2(pos1.x, pos1.y),
		XMFLOAT2(pos2.x, pos2.y),
		TextureNumber[Number]);*/
}
