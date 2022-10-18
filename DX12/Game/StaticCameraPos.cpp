#include "StaticCameraPos.h"

StaticCameraPos::StaticCameraPos()
{
	position = XMFLOAT3(0, 0, 0);
	target = XMFLOAT3(0, 0, 10);
}

StaticCameraPos::~StaticCameraPos()
{
}
