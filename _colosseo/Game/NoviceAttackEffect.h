#pragma once
#include "ActionEffectBase.h"

class NoviceAttackEffect : public ActionEffectBase {

	XMFLOAT3 m_Start;
	XMFLOAT3 m_End;

	float m_Size;
public:
	void SetStartAndEnd(XMFLOAT3 _Start, XMFLOAT3 _End);
	void Start(int _Timer, int _EndTime)  override;
	void Update() override;
	void Draw() override;
};
