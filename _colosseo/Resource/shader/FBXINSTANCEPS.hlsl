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
cbuffer time : register(b7)
{
	uint Time;
	float InterpSize;
	float Focus;
	float FocusSize;
	float Flag;
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
			result.r = max(result.r, 0.0f);
			result.g = max(result.g, 0.0f);
			result.b = max(result.b, 0.0f);
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
			result.r = max(result.r, 0.0f);
			result.g = max(result.g, 0.0f);
			result.b = max(result.b, 0.0f);
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
			result.r = max(result.r, 0.0f);
			result.g = max(result.g, 0.0f);
			result.b = max(result.b, 0.0f);
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
	//if (shadecolor.r <= 0.6f) shadecolor.r = 0.6f;
	if (shadecolor.g > 0.6f) shadecolor.g = 0.9f;
	//if (shadecolor.g <= 0.6f) shadecolor.g = 0.6f;
	if (shadecolor.b > 0.6f) shadecolor.b = 0.9f;
	//if (shadecolor.b <= 0.6f) shadecolor.b = 0.6f;
	shadecolor.r = max(shadecolor.r, 0.6f);
	shadecolor.g = max(shadecolor.g, 0.6f);
	shadecolor.b = max(shadecolor.b, 0.6f);

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 10.0f) {
		DisAlpha = dist / 10.0f;
		/*if (DisAlpha < 0.0f) {
			DisAlpha = 0.0f;
		}*/
		DisAlpha = max(DisAlpha, 0.0f);
	}
	float4 ResultColor = shadecolor * texcolor * InColor;
	ResultColor.a = ResultColor.a * DisAlpha;
	//smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);


	if (ResultColor.a <= 0) {
		discard;
	}


	// シェーディングによる色で描画
	return ResultColor;
}


//----------------- Guide
float3 rgb2hsv(float3 rgb)
{
	float3 hsv;

	// RGBの三つの値で最大のもの
	float maxValue = max(rgb.r, max(rgb.g, rgb.b));
	// RGBの三つの値で最小のもの
	float minValue = min(rgb.r, min(rgb.g, rgb.b));
	// 最大値と最小値の差
	float delta = maxValue - minValue;

	// V（明度）
	// 一番強い色をV値にする
	hsv.z = maxValue;

	// S（彩度）
	// 最大値と最小値の差を正規化して求める
	if (maxValue != 0.0) {
		hsv.y = delta / maxValue;
	}
	else {
		hsv.y = 0.0;
	}

	// H（色相）
	// RGBのうち最大値と最小値の差から求める
	if (hsv.y > 0.0) {
		if (rgb.r == maxValue) {
			hsv.x = (rgb.g - rgb.b) / delta;
		}
		else if (rgb.g == maxValue) {
			hsv.x = 2 + (rgb.b - rgb.r) / delta;
		}
		else {
			hsv.x = 4 + (rgb.r - rgb.g) / delta;
		}
		hsv.x /= 6.0;
		if (hsv.x < 0)
		{
			hsv.x += 1.0;
		}
	}

	return hsv;
}
// HSV->RGB変換
float3 hsv2rgb(float3 hsv)
{
	float3 rgb;

	if (hsv.y == 0) {
		// S（彩度）が0と等しいならば無色もしくは灰色
		rgb.r = rgb.g = rgb.b = hsv.z;
	}
	else {
		// 色環のH（色相）の位置とS（彩度）、V（明度）からRGB値を算出する
		hsv.x *= 6.0;
		float i = floor(hsv.x);
		float f = hsv.x - i;
		float aa = hsv.z * (1 - hsv.y);
		float bb = hsv.z * (1 - (hsv.y * f));
		float cc = hsv.z * (1 - (hsv.y * (1 - f)));
		if (i < 1) {
			rgb.r = hsv.z;
			rgb.g = cc;
			rgb.b = aa;
		}
		else if (i < 2) {
			rgb.r = bb;
			rgb.g = hsv.z;
			rgb.b = aa;
		}
		else if (i < 3) {
			rgb.r = aa;
			rgb.g = hsv.z;
			rgb.b = cc;
		}
		else if (i < 4) {
			rgb.r = aa;
			rgb.g = bb;
			rgb.b = hsv.z;
		}
		else if (i < 5) {
			rgb.r = cc;
			rgb.g = aa;
			rgb.b = hsv.z;
		}
		else {
			rgb.r = hsv.z;
			rgb.g = aa;
			rgb.b = bb;
		}
	}
	return rgb;
}

// 乱数

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

float4 Guide(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	uint index = input.InstanceID;
	matrix world = data[index].world;
	float4 InColor = data[index].InColor;

	if (data[index].DrawFlag == false) {
		discard;
	}

	float4 texcolor = tex.Sample(smp, input.uv);
	float3 TexHsv = rgb2hsv(texcolor.xyz);
	float3 ColorHsv = rgb2hsv(InColor.xyz); 
	TexHsv.x = ColorHsv.x;
	float3 RGB = hsv2rgb(TexHsv);
	float InColW = InColor.w;
	if (InColor.w < 0.0f) {
		InColW = -InColor.w;
		if (input.worldpos.y > 0.1f) {
			discard;
		}
	}
	float4 ResultColor = float4(RGB.x, RGB.y, RGB.z, texcolor.w * InColW);

	//float2 t = float2(input.uv.x, input.uv.x) * (Time * 10.0f);

	int tTime = Time % 30000;
	float2 t = float2(input.uv.x + tTime / 3000.0f, input.uv.x + tTime / 3000.0f) * 1000.0f;

	if (input.worldpos.y > 0.1f) {
		float nn = noise(t);
		if (input.worldpos.y > nn * 4.0f) {
			discard;
		}
		ResultColor.a = 0.51f + nn / 3.0f;
	}

	float DisAlpha = 1.0f;
	float dist = distance(input.worldpos.xyz, cameraPos);
	if (dist < 10.0f) {
		DisAlpha = dist / 10.0f;
		if (DisAlpha < 0.0f) {
			DisAlpha = 0.0f;
		}
	}

	ResultColor.a = ResultColor.a * DisAlpha;

	if (ResultColor.a == 0.0f) discard;

	// シェーディングによる色で描画
	return ResultColor;
}