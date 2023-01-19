#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

cbuffer cbuff0 : register(b0)
{
	matrix view; // ビュー行列
	matrix viewproj; // ビュープロジェクション行列
	matrix viewproj2; // ビュープロジェクション行列2
	matrix world; // ワールド行列
	float3 cameraPos; // カメラ座標（ワールド座標）
	float4 InColor;
};
//バーテックスバッファーの入力
struct VSInput
{
	float4 pos	: POSITION;//位置   
	float3 normal : NORMAL;//頂点法線
	float2 uv	: TEXCOORD;//テクスチャー座標
	min16int4 boneIndices : BONEINDICES; //ボーンの番号
	min16int4 boneIndicesB : BONEINDICESB; //ボーンの番号
	float4 boneWeights : BONEWEIGHTS; //ボーンのスキンウェイト
	float4 boneWeightsB : BONEWEIGHTSB; //ボーンのスキンウェイト
};

cbuffer time : register(b4)
{
	uint Time;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};

float sa(float v1, float v2) {
	float m1 = max(v1, v2);
	float m2 = min(v1, v2);
	return m1 - m2;
}

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcol = tex.Sample(smp, input.uv);
	if (texcol.a < 0.1f) {
		//clip(-1);
		discard;
	}

	if (tex.Sample(smp, input.uv).a == 0.0f) {
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	matrix viewR = view;
	viewR[0][3] = viewR[0][3] - 180.0f;
	viewR[1][3] = viewR[1][3] - 180.0f;
	viewR[2][3] = viewR[2][3] - 180.0f;

	////---------------被写界深度用
	float4 obj = mul(input.worldpos, viewR); //ビュー変換
	float4 cp = float4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	float4 cam = mul(cp, viewR); //ビュー変換

	float3 DOFArea = float3(InterpSize, Focus, FocusSize);
	float3 ParamF = float3(0.0f, 0.0f, 0.0f);

	// 被写界深度の開始位置を計算
	ParamF.x = DOFArea.y - DOFArea.z / 2.0f - DOFArea.x;

	// 被写界深度の終了位置を計算
	ParamF.y = DOFArea.y + DOFArea.z / 2.0f + DOFArea.x;

	// 被写界深度の範囲の逆数を計算
	ParamF.z = 1.0f / (ParamF.y - ParamF.x);

	float4 col = float4(0.0f,0.0f,0.0f,1.0f);

	cam.z = cam.z / cam.w;
	obj.z = obj.z / obj.w;

	// 被写界深度の範囲内を 0.0f ～ 1.0f に変換
	if (sa(cam.z, obj.z) < ParamF.x) {
		col.r = 0.0f;
	}
	else if (sa(cam.z, obj.z) > ParamF.y) {
		col.r = 1.0f;
	}
	else {
		col.r = (sa(cam.z, obj.z) - ParamF.x) * ParamF.z;
	}

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 5.0f) {
		discard;
	}

	///float sss = sa(cam.z, obj.z);
	//return float4(sss / 500.0f, sss / 500.0f, sss / 500.0f, 1);

	return col;
}