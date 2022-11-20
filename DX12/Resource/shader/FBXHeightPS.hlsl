#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> normalMap : register(t1); //1番スロットに設定されたテクスチャ
Texture2D<float4> heightMap : register(t2); //2番スロットに設定されたテクスチャ
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

float4 main(VSOutput input) : SV_TARGET
{
	// 頂点から視点への方向ベクトル(視線ベクトル)
	float3 eyedir = normalize(input.worldpos.xyz - cameraPos);

	//高さマップを参照し、高さを取得する
	float4 heightcolor = heightMap.Sample(smp, input.uv);
	float h = heightcolor.r;
   //高さ情報はアルファ成分に格納されている
   //float h = heightMap.Sample(smp, input.uv).a;

	//高さのパラメータ　大きくすると高さが大きくなる
   float  m_Param = 0.016f * 2.0f;
   //テクセルを頂点座標系での視線ベクトル方向に重みをつけてずらす。
   float2 Tex = input.uv + m_Param * h * eyedir.xy;

	//ノーマルマップから法線を抜き取る
	input.normal = normalMap.Sample(smp, Tex).xyz;

	// テクスチャマッピング
	//float4 texcolor = tex.Sample(smp, input.uv);
	float4 texcolor = tex.Sample(smp, Tex);

	// 光沢度
	const float shininess = 4.0f;

	// 環境反射光
	float3 ambient = m_ambient;

	// シェーディングによる色
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	shadecolor = float4(0, 0, 0, 1);
	float3 dotlightnormal = { 0, 0, 0 };
	float3 reflect = { 0, 0, 0 };
	float3 diffuse = { 0, 0, 0 };
	float3 specular = { 0, 0, 0 };
	float3 lightpos = { 0, 0, 0 };
	float3 lightv = { 0, 0, 0 };
	float d = 0;
	float atten = 0;
	float cos = 0;
	float angleatten = 0;
	float3 casterv = { 0, 0, 0 };
	// 平行光源
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			// ライトに向かうベクトルと法線の内積
			dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// 反射光ベクトル
			reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//影響がマイナスにならないように補正する
			float3 result = (diffuse + specular) * dirLights[i].lightcolor;
			if (result.r < 0.0f) result.r = 0.0f;
			if (result.g < 0.0f) result.g = 0.0f;
			if (result.b < 0.0f) result.b = 0.0f;
			// 全て加算する
			shadecolor.rgb += result;
		}
	}

	// 点光源
	for (i = 0; i < POINTLIGHT_NUM; i++) {
		if (pointLights[i].active) {
			// ライトへの方向ベクトル
			lightv = pointLights[i].lightpos - input.worldpos.xyz;
			d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			// ライトに向かうベクトルと法線の内積
			dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
			reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//影響がマイナスにならないように補正する
			float3 result = atten * (diffuse + specular) * pointLights[i].lightcolor;
			if (result.r < 0.0f) result.r = 0.0f;
			if (result.g < 0.0f) result.g = 0.0f;
			if (result.b < 0.0f) result.b = 0.0f;
			// 全て加算する
			shadecolor.rgb += result;
		}
	}

	// スポットライト
	for (i = 0; i < SPOTLIGHT_NUM; i++) {
		if (spotLights[i].active) {
			// ライトへの方向ベクトル
			lightv = spotLights[i].lightpos - input.worldpos.xyz;
			d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));

			// 角度減衰
			cos = dot(lightv, spotLights[i].lightv);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// ライトに向かうベクトルと法線の内積
			dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
			reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//影響がマイナスにならないように補正する
			float3 result = atten * (diffuse + specular) * spotLights[i].lightcolor;
			if (result.r < 0.0f) result.r = 0.0f;
			if (result.g < 0.0f) result.g = 0.0f;
			if (result.b < 0.0f) result.b = 0.0f;
			// 全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}

	// 丸影
	for (i = 0; i < CIRCLESHADOW_NUM; i++) {
		if (circleShadows[i].active) {
			// オブジェクト表面からキャスターへのベクトル
			casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			// 光線方向での距離
			d = dot(casterv, circleShadows[i].dir);

			// 距離減衰係数
			atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			// 距離がマイナスなら0にする
			atten *= step(0, d);

			// ライトの座標
			lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
			lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
			cos = dot(lightv, circleShadows[i].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}

	if (texcolor.a == 0) discard;

	// シェーディングによる色で描画
	return shadecolor * texcolor;
}