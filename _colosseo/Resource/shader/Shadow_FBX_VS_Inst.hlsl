#include "FBX.hlsli"

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
	uint InstanceID : SV_InstanceID;
};

cbuffer matr : register(b6)
{
	matrix view; // ビュー行列
	matrix viewproj; // ビュープロジェクション行列
	matrix viewproj2; // ビュープロジェクション行列2
	float3 cameraPos; // カメラ座標（ワールド座標）
};

//スキニング後の頂点・法線が入る
struct SkinOutput
{
	float4 pos;
	float3 normal;
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

//スキニング計算
SkinOutput ComputeSkin(VSInput input)
{
	//ゼロクリア
	SkinOutput output = (SkinOutput)0;

	uint iBone; //計算するボーン番号
	float weight; //ボーンウェイト(重み)
	matrix m; //スキニング行列

	//ボーン0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン0
	iBone = input.boneIndicesB.x;
	weight = input.boneWeightsB.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン1
	iBone = input.boneIndicesB.y;
	weight = input.boneWeightsB.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン2
	iBone = input.boneIndicesB.z;
	weight = input.boneWeightsB.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//ボーン3
	iBone = input.boneIndicesB.w;
	weight = input.boneWeightsB.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	return output;
}

VSOutput main(VSInput input)
{

	uint index = input.InstanceID;
	//スキニング計算
//	SkinOutput skinned = ComputeSkin(input);
	//法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(data[index].world, float4(input.normal, 0)));

	//float4 resultPos = skinned.pos; //スキニング後の座標を渡す
	float4 wpos = mul(data[index].world, input.pos); //ワールド変換

	//ピクセルシェーダーに渡す値
	VSOutput output;
	//行列による座標変換
	output.svpos = mul(mul(Light_viewproj, data[index].world), input.pos);
	output.svpos2 = mul(mul(viewproj2, data[index].world), input.pos);
	output.worldpos = wpos;
	//ワールド法線を次のステージに渡す
	output.normal = wnormal.xyz;
	//入力値をそのまま次のステージに渡す
	output.uv = input.uv;
	output.InstanceID = 0;
	if (data[index].uv.x <= -1.0f) {
		return output;
	}
	//入力値をそのまま次のステージに渡す
	if (input.uv.x <= 0.1f && input.uv.y <= 0.1f) {
		output.uv = float2(data[index].uv.x, data[index].uv.y);
	}
	else if (input.uv.x <= 0.1f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.x, data[index].uv.w);
	}
	else if (input.uv.x >= 0.9f && input.uv.y <= 0.1f) {
		output.uv = float2(data[index].uv.z, data[index].uv.y);
	}
	else if (input.uv.x >= 0.9f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.z, data[index].uv.w);
	}

	return output;
}

VSOutput AudMain(VSInput input)
{

	uint index = input.InstanceID;
	//スキニング計算
//	SkinOutput skinned = ComputeSkin(input);
	//法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(data[index].world, float4(input.normal, 0)));

	//float4 resultPos = skinned.pos; //スキニング後の座標を渡す
	float4 wpos = mul(data[index].world, input.pos); //ワールド変換

	//ピクセルシェーダーに渡す値
	VSOutput output;
	//行列による座標変換
	output.svpos = mul(mul(Light_viewproj, data[index].world), input.pos);
	output.svpos2 = mul(mul(viewproj2, data[index].world), input.pos);
	output.worldpos = wpos;
	//ワールド法線を次のステージに渡す
	output.normal = wnormal.xyz;
	//入力値をそのまま次のステージに渡す
	output.uv = input.uv;
	output.InstanceID = 0;
	if (data[index].uv.x <= -1.0f) {
		return output;
	}
	//入力値をそのまま次のステージに渡す
	if (input.uv.x <= 0.1f && input.uv.y <= 0.1f) {
		output.uv = float2(data[index].uv.x, data[index].uv.y);
	}
	else if (input.uv.x <= 0.1f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.x, data[index].uv.w);
	}
	else if (input.uv.x >= 0.9f && input.uv.y <= 0.1f) {
		output.uv = float2(data[index].uv.z, data[index].uv.y);
	}
	else if (input.uv.x >= 0.9f && input.uv.y >= 0.9f) {
		output.uv = float2(data[index].uv.z, data[index].uv.w);
	}

	return output;
}