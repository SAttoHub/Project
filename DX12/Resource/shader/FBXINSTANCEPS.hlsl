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

//float smoothstepf(float edge0, float edge1, float x) {
//	float t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
//	return t * t * (3 - 2 * t);
//}
//
//float smoothstep(float edge0, float edge1, float3 f) {
//	float3 res = { 0,0,0 };
//	res.x = smoothstepf(edge0, edge1, f.x);
//	res.y = smoothstepf(edge0, edge1, f.y);
//	res.z = smoothstepf(edge0, edge1, f.z);
//	return res;
//}

float4 main(VSOutput input) : SV_TARGET
{
	////テクスチャマッピング
	//float4 texcolor = tex.Sample(smp, input.uv);
	////Lambert反射
	//float3 light = normalize(float3(1,-1,1)); //右下奥　向きのライト
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	////陰影とテクスチャの色を合成
	//return shadecolor * texcolor;
	// テクスチャマッピング
	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;
	
	if (data[index].DrawFlag == false) {
		discard;
	}

	float4 texcolor = tex.Sample(smp, input.uv);
	if (InColor.w == -1.0f) {
		return texcolor * float4(InColor.r, InColor.g, InColor.b, 1.0f);
	}
	if (InColor.r == -1.0f) {
		return 1.0f;
	}
	if (InColor.g == -1.0f) {
		return 1.0f;
	}
	if (InColor.b == -1.0f) {
		return 1.0f;
	}

	// 光沢度
	const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

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

	//トゥーン
	float _DiffuseThreshold = 0.1f;
	float _DiffuseBlur = 0.05f;
	float _SpecularThreshold = 0.9f;
	float _SpecularBlur = 0.05f;
	//アンビエントが暗すぎたので値を直接入れる


	// 平行光源
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			// ライトに向かうベクトルと法線の内積
			dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// 反射光ベクトル
			reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//トゥーン
			dotlightnormal = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, dotlightnormal);
			// 拡散反射光
			diffuse = dotlightnormal * m_diffuse;
			//トゥーン2
			float3 ttt = pow(saturate(dot(reflect, eyedir)), shininess);
			ttt = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, ttt);
			// 鏡面反射光
			specular = ttt * m_specular;
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
			//トゥーン
			dotlightnormal = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, dotlightnormal);
			// 拡散反射光
			diffuse = dotlightnormal * m_diffuse;
			//トゥーン2
			float3 ttt = pow(saturate(dot(reflect, eyedir)), shininess);
			ttt = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, ttt);
			// 鏡面反射光
			specular = ttt * m_specular;
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
			//トゥーン
			dotlightnormal = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, dotlightnormal);
			// 拡散反射光
			diffuse = dotlightnormal * m_diffuse;
			//トゥーン2
			float3 ttt = pow(saturate(dot(reflect, eyedir)), shininess);
			ttt = smoothstep(_DiffuseThreshold - _DiffuseBlur, _DiffuseThreshold + _DiffuseBlur, ttt);
			// 鏡面反射光
			specular = ttt * m_specular;
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

	if (shadecolor.r > 0.6f) shadecolor.r = 0.9f;
	if (shadecolor.r <= 0.6f) shadecolor.r = 0.6f;
	if (shadecolor.g > 0.6f) shadecolor.g = 0.9f;
	if (shadecolor.g <= 0.6f) shadecolor.g = 0.6f;
	if (shadecolor.b > 0.6f) shadecolor.b = 0.9f;
	if (shadecolor.b <= 0.6f) shadecolor.b = 0.6f;

	//smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);


	if (texcolor.a == 0) discard;


	// シェーディングによる色で描画
	return shadecolor * texcolor * InColor;
}