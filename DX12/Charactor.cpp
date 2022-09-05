#include "Charactor.h"

Charactor::Charactor()
{
	/*m_MapPos = { 0,0 };
	m_HP = 1;

	int modelData = LoadModelOBJ("Charactor", "mob");
	model = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);*/
}

Charactor::~Charactor()
{
}

void Charactor::ShadowDraw()
{
	ShadowDepthDrawobject3d(model);
}

void Charactor::DepthDraw()
{
	DepthDrawobject3d(model);
}

void Charactor::DOFDepthDraw()
{
	DOFDepthDrawobject3d(model);
}
