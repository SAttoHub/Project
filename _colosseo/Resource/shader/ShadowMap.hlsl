#include "PostEffect.hlsli"
#include "CommonTime.hlsli"
Texture2D<float4> tex : register(t0);
Texture2D<float> LightDepth : register(t1); //ライトから
Texture2D<float> CameraDepth : register(t2); //カメラから
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
    matrix mat;
    float flag;
};

cbuffer cbBuffer1 : register(b1)
{
	//matrix view; // ビュー行列
	matrix viewproj; // ビュープロジェクション行列
    matrix Light_view; // ビュー行列
    matrix Light_viewproj;
    matrix Camera_viewproj_inv;
};

float3 ReconstructWorldPositionFromDepth(
	float2 textureCoord,
	float depth,
	matrix<float, 4, 4> inverseViewProjection)
{
	float4 projectedPosition = float4(
		textureCoord.xy * 2.0 - float2(1.0, 1.0), depth, 1.0);

	float4 position = mul(inverseViewProjection, projectedPosition);
	return position.xyz / position.w;
}

float3 CalcWorldPosFromUVZ(float2 uv, float zInProjectionSpace, float4x4 mViewProjInv)
{
    float3 screenPos;
    // UV座標から射影空間でのxy座標を計算する
    screenPos.xy = (uv * float2(2.0f, -2.0f)) + float2(-1.0f, 1.0f);
    screenPos.z = zInProjectionSpace;

    // 射影空間でのピクセルの座標に、mViewProjInvを乗算して、ワールド座標に変換する
    float4 worldPos = mul(mViewProjInv, float4(screenPos, 1.0f));
    worldPos.xyz /= worldPos.w;
    return worldPos.xyz;
}

float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

float GetRandomNumber(float2 texCoord, int Seed)
{
    return frac(sin(dot(texCoord.xy, float2(12.9898, 78.233)) + Seed) * 43758.5453);
}

float ShadowRandom(float DepthCamera, float2 uv, float4 Wpos) {
    float result = 0.0f;
    for (int i = 0; i < 20; i++) {
        float2 RandPoint;
        RandPoint.x = uv.x + GetRandomNumber(uv, Wpos.x + i) * 0.08f - 0.04f;
        RandPoint.y = uv.y + GetRandomNumber(uv, Wpos.y + i) * 0.08f - 0.04f;
        if (RandPoint.x < 0.0f) {
            RandPoint.x = 0.0f;
        }
        if (RandPoint.x > 1.0f) {
            RandPoint.x = 1.0f;
        }
        if (RandPoint.y < 0.0f) {
            RandPoint.y = 0.0f;
        }
        if (RandPoint.y > 1.0f) {
            RandPoint.y = 1.0f;
        }
        // 深度値からワールド座標を復元
        float3 Wpos_hoge2 = CalcWorldPosFromUVZ(RandPoint, DepthCamera, Camera_viewproj_inv);
        float4 Wpos2 = float4(Wpos_hoge2.x, Wpos_hoge2.y, Wpos_hoge2.z, 1.0f);
        // UVを (-1, 1)(1, -1) から (0, 0)(1, 1) に変更
        float4 obj_shadow2 = mul(Light_viewproj, Wpos2);
        obj_shadow2.xyz = obj_shadow2.xyz / obj_shadow2.w;
        obj_shadow2.xy *= float2(0.5f, -0.5f);
        obj_shadow2.xy += 0.5f;
        float shad_center2 = LightDepth.Sample(smp, obj_shadow2.xy);
        if (shad_center2 < obj_shadow2.z - 0.000462 && obj_shadow2.z < 1.0f) {
            result += 0.05f;
        }
    }
    return result;
}

// 補間関数
float interpolate(float a, float b, float x) {
    float PI = 3.1415926;

    float f = (1.0 - cos(x * PI)) * 0.5;
    return a * (1.0 - f) + b * f;
}

// 乱数生成
float rnd(float2 p) {
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

// 補間乱数
float irnd(float2 p) {
    float2 i = floor(p);
    float2 f = frac(p);
    float4 v = float4(rnd(float2(i.x, i.y)),
        rnd(float2(i.x + 1.0, i.y)),
        rnd(float2(i.x, i.y + 1.0)),
        rnd(float2(i.x + 1.0, i.y + 1.0)));
    return interpolate(interpolate(v.x, v.y, f.x), interpolate(v.z, v.w, f.x), f.y);
}

// ノイズ生成
float noise(float2 p) {
    int   oct = 8;
    float per = 0.5;

    float t = 0.0;
    for (int i = 0; i < oct; i++) {
        float freq = pow(2.0, float(i));
        float amp = pow(per, float(oct - i));
        t += irnd(float2(p.x / freq, p.y / freq)) * amp;
    }
    return t;
}

// シームレスノイズ生成
float snoise(float2 p, float2 q, float2 r) {
    return noise(float2(p.x, p.y)) * q.x * q.y +
        noise(float2(p.x, p.y + r.y)) * q.x * (1.0 - q.y) +
        noise(float2(p.x + r.x, p.y)) * (1.0 - q.x) * q.y +
        noise(float2(p.x + r.x, p.y + r.y)) * (1.0 - q.x) * (1.0 - q.y);
}

float4 main(GSOutput input) : SV_TARGET
{
    float4 Color = tex.Sample(smp, input.uv);
    if (flag == 0.0f) {
        return Color;
    }
    float DepthCamera = CameraDepth.Sample(smp, input.uv);

    //int AtTime = int(Time[0] / 2.0f) % 30000;
   // float2 At = float2(input.uv.x + AtTime / 3000.0f, input.uv.y + AtTime / 3000.0f) * 1000.0f;
    //float Ann = noise(At);
   // return float4(Ann, Ann, Ann, 1);

    // 深度値からワールド座標を復元
    float3 Wpos_hoge = CalcWorldPosFromUVZ(input.uv, DepthCamera, Camera_viewproj_inv);
    float4 Wpos = float4(Wpos_hoge.x, Wpos_hoge.y, Wpos_hoge.z, 1.0f);
    
    // UVを (-1, 1)(1, -1) から (0, 0)(1, 1) に変更
	float4 obj_shadow = mul(Light_viewproj, Wpos);
    obj_shadow.xyz = obj_shadow.xyz / obj_shadow.w;
    obj_shadow.xy *= float2(0.5f, -0.5f);
    obj_shadow.xy += 0.5f;

    float shad_center = LightDepth.Sample(smp, obj_shadow.xy);
    // 深度の差から影かどうかを判定・アクネを軽減
    if (shad_center < obj_shadow.z - 0.000462 && obj_shadow.z < 1.0f) {
        Color.rgb *= 0.5f;
    }
    else {
        int tTime = int(Time[0] / 2.0f) % 30000;
        float2 t = float2(obj_shadow.x + tTime / 3000.0f, obj_shadow.y + tTime / 3000.0f) * 1000.0f;
        float nn = noise(t);
        if (nn < 0.25f) {
            Color.rgb *= 0.7f;
        }
        else if (nn < 0.26f) {
            Color.rgb *= 0.75f;
        }
        else if (nn < 0.27f) {
            Color.rgb *= 0.8f;
        }
        else if (nn < 0.28f) {
            Color.rgb *= 0.85f;
        }
        else if (nn < 0.29f) {
            Color.rgb *= 0.9f;
        }
        else if (nn < 0.3f) {
            Color.rgb *= 0.95f;
        }
    }

	return Color;
}