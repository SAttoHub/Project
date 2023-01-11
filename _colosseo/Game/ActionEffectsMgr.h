#pragma once
#include "ActionEffectBase.h"
#include "../Singleton.h"
#include "MagicAttackEffect.h"
#include "NoviceAttackEffect.h"

class ActionEffectsMgr : public Singleton<ActionEffectsMgr> {
	friend class Singleton<ActionEffectsMgr>;
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	MagicAttackEffect Mg_Eff;
	NoviceAttackEffect Nv_Eff;
	std::vector<ActionEffectBase*> Effects;

public:
	const int MAGIC_EFFECT_TIME = 20;
	const int NOVICE_EFFECT_TIME = 40;

	void StartMagicEffect(XMFLOAT3 _Start, XMFLOAT3 _End);
	void StartNoveceEffect(XMFLOAT3 _Start, XMFLOAT3 _End);

	void Update();
	void Draw();


	// ï°êîÇÃç¿ïWÇÃíÜêSÇéÊìæÇ∑ÇÈ
	XMFLOAT3 GetTargetsCenter(const std::vector<XMFLOAT3>& Targets);
};