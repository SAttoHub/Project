#include "FBX.hlsli"

cbuffer cbuff0 : register(b0)
{
	matrix view; // ビュー行列
	matrix viewproj; // ビュープロジェクション行列
	matrix viewproj2; // ビュープロジェクション行列2
	matrix world; // ワールド行列
	float3 cameraPos; // カメラ座標（ワールド座標）
	float4 InColor;
};

[maxvertexcount(4)]
void main(
	triangle VSOutput input[3] : SV_POSITION,
	inout LineStream< GSOutput > output
)
{
    GSOutput aaa;

	/* ライトへの方向ベクトル
	float3 lightv = cameraPos - input[0].worldpos.xyz;
	float3 d = length(lightv);
	lightv = normalize(lightv);

	 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input[0].worldpos.xyz);
	 光沢度
	const float shininess = 4.0f;


	 ライトに向かうベクトルと法線の内積
	float3 dotlightnormal = dot(lightv, input[0].normal);
	 反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input[0].normal);
	 拡散反射光
	float3 diffuse = dotlightnormal * m_diffuse;
	 鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	影響がマイナスにならないように補正する
	float3 result = (diffuse + specular);
	if (result.r < 0.0f) result.r = 0.0f;
	if (result.g < 0.0f) result.g = 0.0f;
	if (result.b < 0.0f) result.b = 0.0f;*/

	//if (result.r != 0.0f || result.g != 0.0f || result.b != 0.0f) {
		for (uint i = 0; i < 3; i++) {
			aaa.svpos = input[i].svpos;
			aaa.color = float4(0, 0, 0, 1);
			output.Append(aaa);
		}
		aaa.svpos = input[0].svpos;
		aaa.color = float4(0, 0, 0, 1);
		output.Append(aaa);
	//}
}