#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

struct InstanceData
{
	matrix world; // ワールド行列
	float4 InColor;
	float4 uv; // 左上右下
	bool DrawFlag;
};

cbuffer cbuff0 : register(b0)
{
	InstanceData data[500];
};


cbuffer matr : register(b6)
{
	matrix view; // ビュー行列
	matrix viewproj; // ビュープロジェクション行列
	matrix viewproj2; // ビュープロジェクション行列2
	float3 cameraPos; // カメラ座標（ワールド座標）
};

cbuffer time : register(b4)
{
	uint Time;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
};


float4 main(VSOutput input) : SV_TARGET
{
	float4 texcol = tex.Sample(smp, input.uv);
	if (texcol.a < 0.1f) {
		clip(-1);
		//discard;
	}

	if (tex.Sample(smp, input.uv).a == 0.0f) {
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;

	if (data[index].DrawFlag == false) {
		discard;
	}

	////---------------被写界深度用
	float4 obj = mul(input.worldpos, view); //ビュー変換
	float4 cp = float4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	float4 cam = mul(cp, view); //ビュー変換

	float3 DOFArea = float3(InterpSize, Focus, FocusSize);
	float3 ParamF = float3(0.0f, 0.0f, 0.0f);

	// 被写界深度の開始位置を計算
	ParamF.x = DOFArea.y - DOFArea.z / 2.0f - DOFArea.x;

	// 被写界深度の終了位置を計算
	ParamF.y = DOFArea.y + DOFArea.z / 2.0f + DOFArea.x;

	// 被写界深度の範囲の逆数を計算
	ParamF.z = 1.0f / (ParamF.y - ParamF.x);

	float4 col = float4(0.0f,0.0f,0.0f,1.0f);

	// 被写界深度の範囲内を 0.0f ～ 1.0f に変換
	if ((cam.z - obj.z) < ParamF.x) {
		col.r = 0.0f;
	}
	else if ((cam.z - obj.z) > ParamF.y) {
		col.r = 1.0f;
	}
	else {
		col.r = ((cam.z - obj.z) - ParamF.x) * ParamF.z;
	}

	return col;
}