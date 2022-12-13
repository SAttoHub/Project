#pragma once
#include "..\Engine\CommonMathIncludes.h"

//-------------------------------------------------------------------------------------------------------------
// �퓬�V�[���}�b�v�`�b�v�f�[�^
//-------------------------------------------------------------------------------------------------------------
class ChipData {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMINT3 = DirectX::XMINT3;

public:
	static const float CHIP_SIZE;
	XMINT3 m_Pos = XMINT3();
	int Cost = 1;

	void Draw();
};

