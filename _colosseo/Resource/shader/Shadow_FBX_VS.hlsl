#include "FBX.hlsli"

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