#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcol = tex.Sample(smp, input.uv);
	if (texcol.a < 0.1f) {
		clip(-1);
		//discard;
	}
	//-----------------シャドウマップ用
	float4 col = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//float4 obj_shadow = mul(input.worldpos, Light_viewproj); //ビュー変換

	col.r = input.svpos.z / input.svpos.w;

	return col;
}