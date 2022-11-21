#include "PostEffect.hlsli"
Texture2D<float4> tex : register(t0);
Texture2D<float> tex2 : register(t1); //深度
SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix Camera_viewproj_inv;
	float isUse;
};

float3 CalcWorldPosFromUVZ(float2 uv, float zInProjectionSpace, float4x4 mViewProjInv)
{
	float3 screenPos;
	// UV座標から射影空間でのxy座標を計算する
	screenPos.xy = (uv * float2(2.0f, -2.0f)) + float2(-1.0f, 1.0f);
	screenPos.z = zInProjectionSpace;

	// 射影空間でのピクセルの座標に、mViewProjInvを乗算して、ワールド座標に変換する
	float4 worldPos = mul(mViewProjInv, float4(screenPos, 1.0f));
	worldPos.xyz /= worldPos.w;
	return worldPos.xyz;
}

float dist(float3 p1, float3 p2)
{
	float ppx = p2.x - p1.x;
	float ppy = p2.y - p1.y;
	float ppz = p2.z - p1.z;

	return sqrt(ppx * ppx + ppy * ppy + ppz * ppz);
}

float4 main(GSOutput input) : SV_TARGET
{
	float4 Color = tex.Sample(smp, input.uv);

	float Center = tex2.Sample(smp, input.uv);
	float Up = tex2.Sample(smp, input.uv + float2(0,1.0f / 720.0f));
	float Down = tex2.Sample(smp, input.uv + float2(0, -1.0f / 720.0f));
	float Left = tex2.Sample(smp, input.uv + float2(1.0f / 1280.0f, 0));
	float Right = tex2.Sample(smp, input.uv + float2(-1 / 1280.0f, 0));

	/*float3 CenterPos = CalcWorldPosFromUVZ(input.uv, Center, Camera_viewproj_inv);
	float3 UpPos = CalcWorldPosFromUVZ(input.uv, Up, Camera_viewproj_inv);
	float3 DownPos = CalcWorldPosFromUVZ(input.uv, Down, Camera_viewproj_inv);
	float3 LeftPos = CalcWorldPosFromUVZ(input.uv, Left, Camera_viewproj_inv);
	float3 RightPos = CalcWorldPosFromUVZ(input.uv, Right, Camera_viewproj_inv);

	if (dist(CenterPos, UpPos) > 1.0f) {
		return float4(0, 0, 0, 1);
	}
	if (dist(CenterPos, DownPos) > 1.0f) {
		return float4(0, 0, 0, 1);
	}
	if (dist(CenterPos, LeftPos) > 1.0f) {
		return float4(0, 0, 0, 1);
	}
	if (dist(CenterPos, RightPos) > 1.0f) {
		return float4(0, 0, 0, 1);
	}*/


	float pow = 0.0001f;


	if (abs(Center - Up) > pow) {
		return float4(0, 0, 0, 1);
	}
	if (abs(Center - Down) > pow) {
		return float4(0, 0, 0, 1);
	}
	if (abs(Center - Left) > pow) {
		return float4(0, 0, 0, 1);
	}
	if (abs(Center - Right) > pow) {
		return float4(0, 0, 0, 1);
	}

	return Color;
}