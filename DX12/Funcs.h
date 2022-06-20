#pragma once
#include <Windows.h>
#include "ExtendedRandom.h"

/// <summary>
/// マウスカーソルの表示/非表示を切り替える
/// </summary>
/// <param name="Flag">true or false</param>
static void CursorShow(bool Flag) {
	if (Flag) {
		int s = ShowCursor(TRUE);
		while (s < 1) {
			s = ShowCursor(TRUE);
		}
	}
	else {
		int s = ShowCursor(FALSE);
		while (s > -1) {
			s = ShowCursor(FALSE);
		}
	}
}

/// <summary>
/// XMVECTORをXMFLOAT3に変換する
/// </summary>
/// <param name="vec">値</param>
/// <returns></returns>
static inline DirectX::XMFLOAT3 ConvertXMFLOAT3(const DirectX::XMVECTOR &vec) {
	DirectX::XMFLOAT3 result;
	XMStoreFloat3(&result, vec);
	return result;
}

/// <summary>
/// 座標が画面内にあるか
/// </summary>
/// <param name="WorldPos">座標</param>
/// <param name="ViewMat">ビュー行列</param>
/// <param name="ProjMat">プロジェクション行列</param>
/// <param name="WinSize">ウィンドウサイズ</param>
/// <returns></returns>
static inline bool InScreen(const DirectX::XMFLOAT3 WorldPos,
	const DirectX::XMMATRIX &ViewMat, const DirectX::XMMATRIX &ProjMat, const DirectX::XMFLOAT2 &WinSize) {

	// ビューポート行列（スクリーン行列）の作成
	float w = (float)WinSize.x / 2.0f;
	float h = (float)WinSize.y / 2.0f;

	DirectX::XMMATRIX viewport = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w , h , 0 , 1
	};

	DirectX::XMVECTOR screenPos;
	DirectX::XMVECTOR WorldPosV = XMLoadFloat3(&WorldPos);
	// ビュー変換とプロジェクション変換
	WorldPosV = VTransform(WorldPosV, ViewMat);
	WorldPosV = VTransform(WorldPosV, ProjMat);

	// zで割って-1~1の範囲に収める
	WorldPosV.m128_f32[0] /= WorldPosV.m128_f32[2];
	WorldPosV.m128_f32[1] /= WorldPosV.m128_f32[2];
	WorldPosV.m128_f32[2] /= WorldPosV.m128_f32[2];
	// スクリーン変換
	screenPos = VTransform(WorldPosV, viewport);

	DirectX::XMFLOAT2 center;
	XMStoreFloat2(&center, screenPos);

	if (center.x < 0)return false;
	if (WinSize.x < center.x)return false;
	if (center.y < 0)return false;
	if (WinSize.y < center.y)return false;

	return true;
}