#pragma once
#include "SDifines.h"
#include "DirectXMathExpansion.h"
#include <math.h>
#include "Variadic_templates_Func.h"
//参考用チートシートURL　https://easings.net/ja

enum class EaseName {
	Linear, // 線形
	InSine,
	OutSine,
	InOutSine,
	InQuad,
	OutQuad,
	InOutQuad,
	InCubic,
	OutCubic,
	InOutCubic,
	InQuart,
	OutQuart,
	InOutQuart,
	InQuint,
	OutQuint,
	InOutQuint,
	InExpo,
	OutExpo,
	InOutExpo,
	InCirc,
	OutCirc,
	InOutCirc,
	InBack,
	OutBack,
	InOutBack,
	InElastic,
	OutElastic,
	InOutElastic,
	OutBounce,
	InBounce,
	InOutBounce
};

namespace Ease {
	static double Linear(double t) {
		return t / 1.0;
	}
	static double InSine(double t) {
		return 1.0 - cos((t * M_PI_F) / 2.0);
	}
	static double OutSine(double t){
		return sin((t * M_PI_F) / 2.0);
	}
	static double InOutSine(double t){
		return -(cos(M_PI_F * t) - 1.0) / 2.0;
	}
	static double InQuad(double t){
		return t * t;
	}
	static double OutQuad(double t){
		return 1.0 - (1.0 - t) * (1.0 - t);
	}
	static double InOutQuad(double t){
		return t < 0.5 ? 2.0 * t * t : 1.0 - pow(-2.0 * t + 2.0, 2.0) / 2.0;
	}
	static double InCubic(double t){
		return t * t * t;
	}
	static double OutCubic(double t){
		return 1.0 - pow(1.0 - t, 3.0);
	}
	static double InOutCubic(double t){
		return t < 0.5 ? 4.0 * t * t * t : 1.0 - pow(-2.0 * t + 2.0, 3.0) / 2.0;
	}
	static double InQuart(double t){
		return t * t * t * t;
	}
	static double OutQuart(double t){
		return 1.0 - pow(1.0 - t, 4.0);
	}
	static double InOutQuart(double t){
		return t < 0.5 ? 8.0 * t * t * t * t : 1.0 - pow(-2.0 * t + 2.0, 4.0) / 2.0;
	}
	static double InQuint(double t){
		return t * t * t * t * t;
	}
	static double OutQuint(double t){
		return 1.0 - pow(1.0 - t, 5.0);
	}
	static double InOutQuint(double t){
		return t < 0.5 ? 16.0 * t * t * t * t * t : 1.0 - pow(-2.0 * t + 2.0, 5.0) / 2.0;
	}
	static double InExpo(double t){
		return t == 0.0 ? 0.0 : pow(2.0, 10.0 * t - 10.0);
	}
	static double OutExpo(double t){
		return t == 1.0 ? 1.0 : 1.0 - pow(2.0, -10.0 * t);
	}
	static double InOutExpo(double t){
		return t == 0.0
		 ? 0.0
		 : t == 1.0
		 ? 1.0
		 : t < 0.5 ? pow(2.0, 20.0 * t - 10.0) / 2.0
		 : (2.0 - pow(2.0, -20.0 * t + 10.0)) / 2.0;
	}
	static double InCirc(double t){
		return 1.0 - sqrt(1.0 - pow(t, 2.0));
	}
	static double OutCirc(double t){
		return sqrt(1.0 - pow(t - 1.0, 2.0));
	}
	static double InOutCirc(double t){
		return t < 0.5
		  ? (1.0 - sqrt(1.0 - pow(2.0 * t, 2.0))) / 2.0
		 : (sqrt(1.0 - pow(-2.0 * t + 2.0, 2.0)) + 1.0) / 2.0;
	}
	static double InBack(double t){
		const double c1 = 1.70158;
		const double c3 = c1 + 1.0;
		return c3 * t * t * t - c1 * t * t;
	}
	static double OutBack(double t){
		const double c1 = 1.70158;
		const double c3 = c1 + 1.0;
		return 1.0 + c3 * pow(t - 1.0, 3.0) + c1 * pow(t - 1.0, 2.0);
	}
	static double InOutBack(double t){
		const double c1 = 1.70158;
		const double c2 = c1 * 1.525;
		return t < 0.5
		  ? (pow(2.0 * t, 2.0) * ((c2 + 1.0) * 2.0 * t - c2)) / 2.0
		  : (pow(2.0 * t - 2.0, 2.0) * ((c2 + 1.0) * (t * 2.0 - 2.0) + c2) + 2.0) / 2.0;
	}
	static double InElastic(double t){
		const double c4 = (2.0 * M_PI_F) / 3.0;
		return t == 0.0
		  ? 0.0
		  : t == 1.0
		  ? 1.0
		  : -pow(2.0, 10.0 * t - 10.0) * sin((t * 10.0 - 10.75) * c4);
	}
	static double OutElastic(double t){
		const double c4 = (2.0 * M_PI_F) / 3.0;
		return t == 0.0
		  ? 0.0
		  : t == 1.0
		  ? 1.0
		  : pow(2.0, -10.0 * t) * sin((t * 10.0 - 0.75) * c4) + 1.0;
	}
	static double InOutElastic(double t){
		const double c5 = (2.0 * M_PI_F) / 4.5;
		return t == 0.0
		  ? 0.0
		  : t == 1.0
		  ? 1.0
		  : t < 0.5
		  ? -(pow(2.0, 20.0 * t - 10.0) * sin((20.0 * t - 11.125) * c5)) / 2.0
		  : (pow(2.0, -20.0 * t + 10.0) * sin((20.0 * t - 11.125) * c5)) / 2.0 + 1.0;
	}
	static double OutBounce(double t){
		const double n1 = 7.5625;
		const double d1 = 2.75;
		if (t < 1.0 / d1) {
			return n1 * t * t;
		}
		 else if (t < 2.0 / d1) {
		 return n1 * (t -= 1.5 / d1) * t + 0.75;
		}
		 else if (t < 2.5 / d1) {
		  return n1 * (t -= 2.25 / d1) * t + 0.9375;
		}
		 else {
		 return n1 * (t -= 2.625 / d1) * t + 0.984375;
		}
	}
	static double InBounce(double t) {
		return 1.0 - OutBounce(1.0 - t);
	}
	static double InOutBounce(double t){
		return t < 0.5
		  ? (1.0 - OutBounce(1.0 - 2.0 * t)) / 2.0
		  : (1.0 + OutBounce(2.0 * t - 1.0)) / 2.0;
	}
	static double EaseFuncOrigin(EaseName type, double t) {
		double result = 0.0;
		switch (type) {
		case EaseName::Linear:
			result = Linear(t);
			break;
		case EaseName::InSine:
			result = InSine(t);
			break;
		case EaseName::OutSine:
			result = OutSine(t);
			break;
		case EaseName::InOutSine:
			result = InOutSine(t);
			break;
		case EaseName::InQuad:
			result = InQuad(t);
			break;
		case EaseName::OutQuad:
			result = OutQuad(t);
			break;
		case EaseName::InOutQuad:
			result = InOutQuad(t);
			break;
		case EaseName::InCubic:
			result = InCubic(t);
			break;
		case EaseName::OutCubic:
			result = OutCubic(t);
			break;
		case EaseName::InOutCubic:
			result = InOutCubic(t);
			break;
		case EaseName::InQuart:
			result = InQuart(t);
			break;
		case EaseName::OutQuart:
			result = OutQuart(t);
			break;
		case EaseName::InOutQuart:
			result = InOutQuart(t);
			break;
		case EaseName::InQuint:
			result = InQuint(t);
			break;
		case EaseName::OutQuint:
			result = OutQuint(t);
			break;
		case EaseName::InOutQuint:
			result = InOutQuint(t);
			break;
		case EaseName::InExpo:
			result = InExpo(t);
			break;
		case EaseName::OutExpo:
			result = OutExpo(t);
			break;
		case EaseName::InOutExpo:
			result = InOutExpo(t);
			break;
		case EaseName::InCirc:
			result = InCirc(t);
			break;
		case EaseName::OutCirc:
			result = OutCirc(t);
			break;
		case EaseName::InOutCirc:
			result = InOutCirc(t);
			break;
		case EaseName::InBack:
			result = InBack(t);
			break;
		case EaseName::OutBack:
			result = OutBack(t);
			break;
		case EaseName::InOutBack:
			result = InOutBack(t);
			break;
		case EaseName::InElastic:
			result = InElastic(t);
			break;
		case EaseName::OutElastic:
			result = OutElastic(t);
			break;
		case EaseName::InOutElastic:
			result = InOutElastic(t);
			break;
		case EaseName::OutBounce:
			result = OutBounce(t);
			break;
		case EaseName::InBounce:
			result = InBounce(t);
			break;
		case EaseName::InOutBounce:
			result = InOutBounce(t);
			break;
		default:
			break;
		}
		return result;
	}

	static double EaseFunc(EaseName type, double start, double end, double Now, double Max) {
		double t = Now / Max;
		return start + (EaseFuncOrigin(type, t) * (end - start));
	}

	static DirectX::XMFLOAT2 EaseFunc(EaseName type, DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end, double Now, double Max) {
		double t = Now / Max;
		DirectX::XMFLOAT2 result = DirectX::XMFLOAT2(
			start.x + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.x - start.x)),
			start.y + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.y - start.y))
		);
		return result;
	}

	static DirectX::XMFLOAT3 EaseFunc(EaseName type, DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, double Now, double Max) {
		double t = Now / Max;
		DirectX::XMFLOAT3 result = DirectX::XMFLOAT3(
			start.x + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.x - start.x)),
			start.y + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.y - start.y)),
			start.z + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.z - start.z))
		);
		return result;
	}

	static DirectX::XMFLOAT4 EaseFunc(EaseName type, DirectX::XMFLOAT4 start, DirectX::XMFLOAT4 end, double Now, double Max) {
		double t = Now / Max;
		DirectX::XMFLOAT4 result = DirectX::XMFLOAT4(
			start.x + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.x - start.x)),
			start.y + static_cast<float> (EaseFuncOrigin(type, t) * static_cast<double>(end.y - start.y)),
			start.z + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.z - start.z)),
			start.w + static_cast<float>(EaseFuncOrigin(type, t) * static_cast<double>(end.w - start.w))
		);
		return result;
	}

	// スコアとかのぐーーっと上に上がっていくやつとかに使う
	template<class... T>
	static double StepEase(double Now, T... Thresholds) {
		// しきい値の個数を取得
		int Count = int(sizeof...(Thresholds));
		double Threshold_Up = 0.0; // Now のひとつ上のしきい値
		double Threshold_Bot = 0.0; // Now のひとつ下のしきい値
		double Res = 0.0;
		for (int i = 1; i < Count; i++) {
			Threshold_Bot = Threshold_Up;
			Threshold_Up = Variadic_templates_Func::get(i, Thresholds...);
			if (Now > Threshold_Bot && Now <= Threshold_Up) {
				double Result = 0.0;
				Result = static_cast<double>(i - 1) + ((Now - Threshold_Bot) / (Threshold_Up - Threshold_Bot));
				Res = Result;
			}
		}
		return Res / static_cast<double>(Count - 1);
	}
}