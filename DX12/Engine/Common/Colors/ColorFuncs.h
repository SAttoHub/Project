#pragma once
#include <DirectXMath.h>
#include "ColorDefines.h"
namespace {
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
}

#pragma region ŠÖ”
#pragma region RGB‚ÆRGBA‚Ì‘ŠŒİ•ÏŠ·
/// <summary>
/// XMFLOAT4(RGBA)‚ğXMFLOAT3(RGB)‚É•ÏŠ·
/// </summary>
/// <param name="color">F</param>
/// <returns></returns>
static XMFLOAT3 RGBAConvertRGB(const XMFLOAT4 color) {
	XMFLOAT3 result = { color.x, color.y, color.z };
	return result;
}
/// <summary>
/// XMFLOAT3(RGB)‚ğXMFLOAT4(RGBA)‚É•ÏŠ·
/// </summary>
/// <param name="color">F</param>
/// <param name="alpha">ƒAƒ‹ƒtƒ@’l</param>
/// <returns></returns>
static XMFLOAT4 RGBConvertRGBA(const XMFLOAT3 color, float alpha = 1.0f) {
	XMFLOAT4 result = { color.x, color.y, color.z , alpha};
	return result;
}
#pragma endregion

#pragma region 0 ` 255 ‚ÌF‚ğ0`1‚ÉŠÛ‚ß‚é
/// <summary>
/// 0 ` 255 ‚ÌF‚ğ0`1‚ÉŠÛ‚ß‚é
/// </summary>
/// <param name="color">F</param>
/// <returns></returns>
static XMFLOAT4 ColorConvert(const XMFLOAT4 color) {
	XMFLOAT4 anser = color;
	//color‚ğ0`1‚ÉŠÛ‚ß‚é
	anser.x < 0 ? anser.x = 0 : anser.x > 255 ? anser.x = 255 : 0;
	anser.y < 0 ? anser.y = 0 : anser.y > 255 ? anser.y = 255 : 0;
	anser.z < 0 ? anser.z = 0 : anser.z > 255 ? anser.z = 255 : 0;
	anser.w < 0 ? anser.w = 0 : anser.w > 255 ? anser.w = 255 : 0;
	anser.x = anser.x / 255.0f;
	anser.y = anser.y / 255.0f;
	anser.z = anser.z / 255.0f;
	anser.w = anser.w / 255.0f;
	return anser;
}
/// <summary>
/// 0 ` 255 ‚ÌF‚ğ0`1‚ÉŠÛ‚ß‚é
/// </summary>
/// <param name="color">F</param>
/// <returns></returns>
static XMFLOAT3 ColorConvert(const XMFLOAT3 color) {
	XMFLOAT3 anser = color;
	//color‚ğ0`1‚ÉŠÛ‚ß‚é
	anser.x < 0 ? anser.x = 0 : anser.x > 255 ? anser.x = 255 : 0;
	anser.y < 0 ? anser.y = 0 : anser.y > 255 ? anser.y = 255 : 0;
	anser.z < 0 ? anser.z = 0 : anser.z > 255 ? anser.z = 255 : 0;
	anser.x = anser.x / 255.0f;
	anser.y = anser.y / 255.0f;
	anser.z = anser.z / 255.0f;
	return anser;
}
/// <summary>
/// 0 ` 255 ‚ÌF‚ğ0`1‚ÉŠÛ‚ß‚é
/// </summary>
/// <param name="color">F</param>
/// <returns></returns>
static XMFLOAT4 ColorConvert(const float R, const float G, const float B, const float A) {
	XMFLOAT4 anser = { R,G,B,A };
	//color‚ğ0`1‚ÉŠÛ‚ß‚é
	anser.x < 0 ? anser.x = 0 : anser.x > 255 ? anser.x = 255 : 0;
	anser.y < 0 ? anser.y = 0 : anser.y > 255 ? anser.y = 255 : 0;
	anser.z < 0 ? anser.z = 0 : anser.z > 255 ? anser.z = 255 : 0;
	anser.w < 0 ? anser.w = 0 : anser.w > 255 ? anser.w = 255 : 0;
	anser.x = anser.x / 255.0f;
	anser.y = anser.y / 255.0f;
	anser.z = anser.z / 255.0f;
	anser.w = anser.w / 255.0f;
	return anser;
}
/// <summary>
/// 0 ` 255 ‚ÌF‚ğ0`1‚ÉŠÛ‚ß‚é
/// </summary>
/// <param name="color">F</param>
/// <returns></returns>
static XMFLOAT3 ColorConvert(const float R, const float G, const float B) {
	XMFLOAT3 anser = { R,G,B };
	//color‚ğ0`1‚ÉŠÛ‚ß‚é
	anser.x < 0 ? anser.x = 0 : anser.x > 255 ? anser.x = 255 : 0;
	anser.y < 0 ? anser.y = 0 : anser.y > 255 ? anser.y = 255 : 0;
	anser.z < 0 ? anser.z = 0 : anser.z > 255 ? anser.z = 255 : 0;
	anser.x = anser.x / 255.0f;
	anser.y = anser.y / 255.0f;
	anser.z = anser.z / 255.0f;
	return anser;
}
#pragma endregion

#pragma region 0`1 ‚ÌF‚ğ0 ` 255‚É•ÏŠ·
/// <summary>
/// 0`1 ‚ÌF‚ğ0 ` 255‚É•ÏŠ·
/// </summary>
/// <param name="color">F</param>
/// <returns></returns>
static XMFLOAT4 ColorConvert2(const XMFLOAT4 color) {
	XMFLOAT4 anser = color;
	//color‚ğ0`1‚ÉŠÛ‚ß‚é
	anser.x < 0 ? anser.x = 0 : anser.x > 1 ? anser.x = 1 : 0;
	anser.y < 0 ? anser.y = 0 : anser.y > 1 ? anser.y = 1 : 0;
	anser.z < 0 ? anser.z = 0 : anser.z > 1 ? anser.z = 1 : 0;
	anser.w < 0 ? anser.w = 0 : anser.w > 1 ? anser.w = 1 : 0;
	anser.x = anser.x * 255.0f;
	anser.y = anser.y * 255.0f;
	anser.z = anser.z * 255.0f;
	anser.w = anser.w * 255.0f;
	return anser;
}
/// <summary>
/// 0`1 ‚ÌF‚ğ0 ` 255‚É•ÏŠ·
/// </summary>
/// <param name="color">F</param>
/// <returns></returns>
static XMFLOAT3 ColorConvert2(const XMFLOAT3 color) {
	XMFLOAT3 anser = color;
	//color‚ğ0`1‚ÉŠÛ‚ß‚é
	anser.x < 0 ? anser.x = 0 : anser.x > 1 ? anser.x = 1 : 0;
	anser.y < 0 ? anser.y = 0 : anser.y > 1 ? anser.y = 1 : 0;
	anser.z < 0 ? anser.z = 0 : anser.z > 1 ? anser.z = 1 : 0;
	anser.x = anser.x * 255.0f;
	anser.y = anser.y * 255.0f;
	anser.z = anser.z * 255.0f;
	return anser;
}
#pragma endregion

#pragma region HSV <--> RGB

static DirectX::XMFLOAT3 rgb2hsv(DirectX::XMFLOAT3 rgb)
{
	//DirectX::XMFLOAT3  hsv;
	//float   rgb_max, rgb_min;
	//float   diff;
	//float   base;
	//float   div1;
	//float   div2;

	//rgb_max = max(max(rgb.x, rgb.y), rgb.z);
	//rgb_min = min(min(rgb.x, rgb.y), rgb.z);
	//if (rgb_max == rgb.x) {
	//	diff = rgb.y - rgb.z;   // G-B
	//	base = (rgb.y < rgb.z) ? 360.0f : 0.0f;
	//}
	//if (rgb_max == rgb.y) {
	//	diff = rgb.z - rgb.x;   // B-R
	//	base = 120.0f;
	//}
	//if (rgb_max == rgb.z) {
	//	diff = rgb.x - rgb.y;   // R-G
	//	base = 240.0f;
	//}

	//div1 = (rgb_max == rgb_min) ? 1.0f : (rgb_max - rgb_min);
	//div2 = (rgb_max > 0.0f) ? rgb_max : 1.0f;
	//hsv.x = 60.0f * diff / div1 + base;
	//hsv.y = (rgb_max - rgb_min) / div2;
	//hsv.z = rgb_max;

	return DirectX::XMFLOAT3();
	//return hsv;
}

static DirectX::XMFLOAT3 hsv2rgb(DirectX::XMFLOAT3 hsv)
{
	DirectX::XMFLOAT3  rgb = XMFLOAT3();
	int     Hi;
	float   f;
	float   p;
	float   q;
	float   t;

	Hi = int(fmod(floor(hsv.x / 60.0f), 6.0f));
	f = hsv.x / 60.0f - Hi;
	p = hsv.z * (1.0f - hsv.y);
	q = hsv.z * (1.0f - f * hsv.y);
	t = hsv.z * (1.0f - (1.0f - f) * hsv.y);

	if (Hi == 0) {
		rgb.x = hsv.z;
		rgb.y = t;
		rgb.z = p;
	}
	if (Hi == 1) {
		rgb.x = q;
		rgb.y = hsv.z;
		rgb.z = p;
	}
	if (Hi == 2) {
		rgb.x = p;
		rgb.y = hsv.z;
		rgb.z = t;
	}
	if (Hi == 3) {
		rgb.x = p;
		rgb.y = q;
		rgb.z = hsv.z;
	}
	if (Hi == 4) {
		rgb.x = t;
		rgb.y = p;
		rgb.z = hsv.z;
	}
	if (Hi == 5) {
		rgb.x = hsv.z;
		rgb.y = p;
		rgb.z = q;
	}

	return rgb;
}
#pragma endregion

#pragma endregion