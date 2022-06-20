#pragma once
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ease.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <DirectXMath.h>
#include "DirectXMathExpansion.h"
#include "SDifines.h"

namespace Smath {
	/// <summary>
	/// 最短の弧を描くように角度を変える場合の角度の加算値を求める
	/// </summary>
	/// <param name="NowAngle">現在の角度</param>
	/// <param name="EndAngle">目標の角度</param>
	static float RotateEarliestArc(float NowAngle, float EndAngle) {
		//角度の差が180を超えている場合に最短の弧を求める
		if (fabsf(EndAngle - NowAngle) > 180.0f) {
			if (NowAngle < 180.0f) {
				NowAngle += 360.0f;
			}
			else {
				NowAngle -= 360.0f;
			}
		}
		return EndAngle - NowAngle;
	}

	/// <summary>
	/// 角度を0～360に丸める
	/// </summary>
	/// <param name="Angle">角度</param>
	/// <returns></returns>
	static float ToRoundAngle(float Angle) {
		float Decimal = Angle - (int)Angle;
		if (Angle > 360) {
			Angle = ((int)Angle % 360) + Decimal;
		}
		else if (Angle < 0) {
			Angle = 360.0f - (float)(((int)Angle % 360) + Decimal);
		}
		return Angle;
	}

	/// <summary>
	/// ラジアンを度に変換
	/// </summary>
	/// <param name="radian">ラジアン</param>
	static double degrees(double radian) {
		//α＝180 / π×θ
		return (180.0 / M_PI) * radian;
	}

	/// <summary>
	/// 度をラジアンに変換
	/// </summary>
	/// <param name="radian">ラジアン</param>
	static double radian(double radian) {
		//α＝π / 180×θ
		return (M_PI / 180.0) * radian;
	}


	// 2つの数値をatan2でラジアンにして角度に変換(上方向が0度)
	static double atan2ToDegrees(double X, double Y) {
		if (degrees(atan2(X, Y) < 0)) {
			return degrees(atan2(X, Y)) + 360.0;
		}
		else {
			return degrees(atan2(X, Y));
		}
	}

	static double distance_d(double x1, double y1, double z1, double x2, double y2, double z2)
	{
		double l;
		double dx, dy, dz;
		// x座標の差を計算してdxに代入
		dx = x2 - x1;
		// y座標の差を計算してdyに代入
		dy = y2 - y1;
		// y座標の差を計算してdyに代入
		dz = z2 - z1;
		
		l = sqrt(dx * dx + dy * dy + dz * dz);
		
		return l;
	}

	//点同士の距離を求める
	static double distance_XMF3(const DirectX::XMFLOAT3 p1, const DirectX::XMFLOAT3 p2)
	{
		double l;
		double dx, dy, dz;
		// x座標の差を計算してdxに代入
		dx = (double)p2.x - (double)p1.x;
		// y座標の差を計算してdyに代入
		dy = (double)p2.y - (double)p1.y;
		// y座標の差を計算してdyに代入
		dz = (double)p2.z - (double)p1.z;

		l = sqrt(dx * dx + dy * dy + dz * dz);

		return l;
	}

	//値を範囲内に収める [low, high]
	static float clamp(float value, float low, float high) {
		value = (value < low) ? low : value;
		value = (value > high) ? high : value;
		return value;
	}

	/// <summary>
	/// 値の指定での範囲の0～1の割合を求める
	/// </summary>
	/// <param name="Value">値</param>
	/// <param name="low">下限</param>
	/// <param name="high">上限</param>
	/// <returns></returns>
	inline float CalPercentageZeroToOne(const float &value, const float &low, const float &high) {
		float max = high;
		float now = value;
		//最小値が0未満の場合は最大値に足す
		if (low < 0.0f) {
			max += fabs(low);
			now += fabs(low);
		}
		//最小値が0より大きい場合は最大値から引く
		else if (low > 0.0f) {
			max -= fabs(low);
			now -= fabs(low);
		}

		float result = now / max;

		//0以下1以上にならないようにする
		if (result <= 0.0f) result = 0.0f;
		if (result >= 1.0f) result = 1.0f;

		return result;

	}

	/// <summary>
	/// 角度を単位方向ベクトルに変換
	/// </summary>
	/// <param name="Angle">角度</param>
	/// <returns></returns>
	static DirectX::XMFLOAT2 AngleToDirectionVector(float Angle) {
		float Radian = (float)radian(Angle);
		DirectX::XMFLOAT2 result = { cosf(Radian),sinf(Radian) };
		return result;
	}

	/// <summary>
	/// pos1がpos1frontの方向を向いている時にpos2が左右どちらに居るかを求める
	/// false なら 左  :  true なら 右
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos1front"></param>
	/// <param name="pos2"></param>
	/// <returns></returns>
	static bool GetDirection(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 pos1front, DirectX::XMFLOAT2 pos2) {
		float cross = Cross2D(pos1front, pos2 - pos1);
		return cross > 0;
	}

	/// <summary>
	/// 方向ベクトルを回転させる
	/// </summary>
	/// <param name="dir">方向ベクトル</param>
	/// <param name="rotate">回転角度</param>
	/// <returns></returns>
	static DirectX::XMFLOAT2 RotateVector(DirectX::XMFLOAT2 dir, float rotate) {
		float rot = rotate * (M_PI_F / 180.0f);
		float rx = cosf(rot) * dir.x - sinf(rot) * dir.y;
		float ry = sinf(rot) * dir.x + cosf(rot) * dir.y;
		return  DirectX::XMFLOAT2(rx, ry);
	}

	/// <summary>
	/// valueのm桁目からn桁目を取得する
	/// </summary>
	/// <param name="value">取得先となる浮動小数点数</param>
	/// <param name="m">取得を開始する桁</param>
	/// <param name="n">取得を終了する桁</param>
	/// <returns></returns>
	static int GetDigits(int value, int m, int n) {
		double mod_value;
		int result;
		// 絶対値を求める
		value = int(fabs(value));
		// n桁目以下の桁を取得
		mod_value = fmod(value, pow(10, n + 1));
		// m桁目以上の桁を取得
		result = int(mod_value / pow(10, m));

		return result;
	}
}