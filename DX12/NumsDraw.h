#pragma once
#include <string>
#include "Engine/Common/DirectXMath/DirectXMathExpansion.h"
#include "Singleton.h"

// 数字専用DrawGraph実行クラス
class NumsDraw : public Singleton<NumsDraw> {
	friend class Singleton<NumsDraw>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	//int TextureNumber[9];

private:
	void LoadNumderTexture();

public:
	NumsDraw();
	~NumsDraw(){}
	/// <summary>
	/// 指定範囲内に収まるように数字を描画する
	/// </summary>
	/// <param name="pos1">左上座標</param>
	/// <param name="pos2">右下座標</param>
	/// <param name="Number">数字</param>
	void DrawNumber(XMFLOAT2 pos1, XMFLOAT2 pos2, int Number, XMFLOAT4 color);

};

/// <summary>
/// 指定範囲内に収まるように数字を描画する
/// </summary>
/// <param name="pos1">左上座標</param>
/// <param name="pos2">右下座標</param>
/// <param name="Number">数字</param>
static void DrawNumber(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 pos2, int Number, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1,1,1,1)) {
	NumsDraw::Instance()->DrawNumber(pos1, pos2, Number, color);
}

