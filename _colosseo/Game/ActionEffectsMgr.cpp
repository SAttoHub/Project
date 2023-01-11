#include "ActionEffectsMgr.h"
#include "MagicAttackEffect.h"

void ActionEffectsMgr::StartMagicEffect(XMFLOAT3 _Start, XMFLOAT3 _End)
{
    Mg_Eff.SetStartAndEnd(_Start, _End);
    Mg_Eff.Start(0, MAGIC_EFFECT_TIME);
    Effects.emplace_back(&Mg_Eff);
}

void ActionEffectsMgr::StartNoveceEffect(XMFLOAT3 _Start, XMFLOAT3 _End)
{
    Nv_Eff.SetStartAndEnd(_Start, _End);
    Nv_Eff.Start(0, NOVICE_EFFECT_TIME);
    Effects.emplace_back(&Nv_Eff);
}

void ActionEffectsMgr::Update()
{
    for (int i = 0; i < Effects.size(); i++) {
        if (Effects[i]->m_isEnd == true) {
            Effects.erase(Effects.begin() + i);
            break;
        }
    }

    for (auto& data : Effects) {
        data->Update();
    }
}

void ActionEffectsMgr::Draw()
{
    for (auto& data : Effects) {
        data->Draw();
    }
}

DirectX::XMFLOAT3 ActionEffectsMgr::GetTargetsCenter(const std::vector<XMFLOAT3>& Targets)
{
    XMFLOAT3 Result = XMFLOAT3();

    for (auto& data : Targets) {
        Result += data;
    }
    Result = Result / float(Targets.size());

    return Result;
}
