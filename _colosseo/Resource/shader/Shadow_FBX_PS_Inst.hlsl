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

cbuffer shadow : register(b4)
{
	matrix Light_view; // ビュー行列
	matrix Light_viewproj;
	float3 Light_Pos;
};

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcol = tex.Sample(smp, input.uv);
	if (texcol.a < 0.1f) {
		//clip(-1);
		discard;
	}
	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;

	if (data[index].DrawFlag == false) {
		discard;
	}

	float4 col = float4(0.0f,0.0f,0.0f,1.0f);

	//-----------------シャドウマップ用
	float4 obj_shadow = mul(input.worldpos, Light_viewproj); //ビュー変換

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 5.0f) {
		discard;
	}

	col.r = input.svpos.z / input.svpos.w;

	return col;
}