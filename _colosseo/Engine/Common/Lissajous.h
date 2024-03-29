#pragma once
#include <math.h>
#include "SDifines.h"
#include "DirectXMath/DirectXMathExpansion.h"

namespace Lissajous {
	/// <summary>
	/// T[WÈü
	/// </summary>
	/// <param name="a">püg a</param>
	/// <param name="b">püg b</param>
	/// <param name="fi">Ê· fi</param>
	/// <param name="t">lt</param>
	/// <returns></returns>
	static DirectX::XMFLOAT2 Lissajou(double a, double b, double fi, double t) {
		DirectX::XMFLOAT2 result;
		result.x = sin((a * t) + fi);
		result.y = sin(b * t);
		return result;
	}
}