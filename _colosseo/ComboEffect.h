#pragma once
#include "Effect.h"
class ComboEffect :
    public Effect
{
    XMFLOAT2 Pos;
    float alpha;
    int Combo;

    float scale;
    float P;
public:
    ComboEffect(XMFLOAT2 p1, int EndTime, int Combo);
    ~ComboEffect();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
};

