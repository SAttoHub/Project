#include "FBX.hlsli"
#include "CommonTime.hlsli"
//スキニング後の頂点・法線が入る
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

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

//ボーンの最大数
static const int MAX_BONES = 61;

cbuffer skinning:register(b3)//ボーンのスキニング行列が入る
{
	matrix matSkinning[MAX_BONES];
};

cbuffer shadow : register(b5)
{
	matrix Light_view; // ビュー行列
	matrix Light_viewproj;
	float3 Light_Pos;
};

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



VSOutput main(VSInput input)
{
	////スキニング計算
	//SkinOutput skinned = ComputeSkin(input);
	////法線にワールド行列によるスケーリング・回転を適用
	//float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));

	//float4 resultPos = skinned.pos; //スキニング後の座標を渡す
	//float4 wpos = mul(world, resultPos); //ワールド変換

	////ピクセルシェーダーに渡す値
	//VSOutput output;
	////行列による座標変換
	//output.svpos = mul(mul(Light_viewproj, world), skinned.pos);
	//output.svpos2 = mul(mul(viewproj2, world), skinned.pos);
	//output.worldpos = wpos;
	////ワールド法線を次のステージに渡す
	//output.normal = wnormal.xyz;
	////入力値をそのまま次のステージに渡す
	//output.uv = input.uv;
	//output.InstanceID = 0;

	//return output;


	//法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(input.normal, 0)));

	uint tTime = uint(Time[0] * 2.0f) % 30000;
	float2 t = float2(input.pos.x + input.pos.y + tTime / 3000.0f, input.pos.z + input.pos.y + tTime / 3000.0f) * 1000.0f;
	float nn = noise(t);
	float power = 1.0f - input.pos.y / 4.0f;
	power = min(power, 1.0f);
	input.pos.x = input.pos.x + (nn * input.normal.x * power * 0.3f);
	input.pos.z = input.pos.z + (nn * input.normal.z * power * 0.3f);

	//float4 resultPos = skinned.pos; //スキニング後の座標を渡す
	float4 wpos = mul(world, input.pos); //ワールド変換

	//ピクセルシェーダーに渡す値
	VSOutput output;
	//行列による座標変換
	output.svpos = mul(mul(Light_viewproj, world), input.pos);
	output.svpos2 = mul(mul(viewproj2, world), input.pos);
	output.worldpos = wpos;
	//ワールド法線を次のステージに渡す
	output.normal = wnormal.xyz;
	//入力値をそのまま次のステージに渡す
	output.uv = input.uv;
	output.InstanceID = 0;

	return output;
}