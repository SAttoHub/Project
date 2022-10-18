#include "ChipData.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\Engine\Common\Colors\ColorFuncs.h"

const float ChipData::CHIP_SIZE = 32.0f / 4.0f;

void ChipData::Draw()
{
	DrawLine(XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE),
		XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE),
		ColorConvert(0, 0, 0, 255));

	DrawLine(XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE),
		XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE),
		ColorConvert(0, 0, 0, 255));

	DrawLine(XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE),
		XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE),
		ColorConvert(0, 0, 0, 255));

	DrawLine(XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE),
		XMFLOAT3(float(m_Pos.x) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE, float(m_Pos.y), float(m_Pos.z) * ChipData::CHIP_SIZE + ChipData::CHIP_SIZE),
		ColorConvert(0, 0, 0, 255));
}
