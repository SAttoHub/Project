#include "PostEffect.hlsli"
#include "CommonTime.hlsli"
Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
    matrix mat;
};

float circle(float2 p, float r)
{
    return length(p) - r;
}

float distSquared(float2 A, float2 B)
{
    float2 C = A - B;
    return dot(C, C);
}

float4 main(GSOutput input) : SV_TARGET
{
     float4 Color = tex.Sample(smp, input.uv);
    float4 Color2 = tex2.Sample(smp, input.uv);
    /*if (input.uv.x < 0.5f) {
        return Color;
    }
    return Color2;*/


    float fade = abs(sin(Time[1]));

    float2 iResolution = float2(1280.0f, 720.0f);
    float2 iMouse = float2(0.0f, 720.0f);

    float2 mousePos = iMouse.xy / iResolution.xy * 2.0f - 1.0f;
    float aspect = iResolution.x / iResolution.y;
    float2 fadeTargetPos = float2(mousePos.x * aspect, mousePos.y);
    float maxDist;
    maxDist = distSquared(fadeTargetPos, float2(-1.0f * aspect, -1.0f));
    maxDist = max(maxDist, distSquared(fadeTargetPos, float2(1.0f * aspect, -1.0f)));
    maxDist = max(maxDist, distSquared(fadeTargetPos, float2(1.0f * aspect, 1.0f)));
    maxDist = max(maxDist, distSquared(fadeTargetPos, float2(-1.0f * aspect, 1.0f)));
    maxDist = sqrt(maxDist);
    float2 fragCoord = float2(0, 0);
    float2 uv = (2. * fragCoord - iResolution.xy) / iResolution.y;
    uv += fadeTargetPos * -1.;
    float fadePower = step(0.001f, circle(uv, (fade)*maxDist));

    //float2 col = float2(fadePower);
   

    if (fadePower < 0.1f) {
        Color.w = 1.0f;
        return Color;
    }
    Color2.w = 1.0f;
    return Color2;

    //fragColor = float4(col, 1.0);
}