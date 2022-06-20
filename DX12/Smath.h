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
	/// �ŒZ�̌ʂ�`���悤�Ɋp�x��ς���ꍇ�̊p�x�̉��Z�l�����߂�
	/// </summary>
	/// <param name="NowAngle">���݂̊p�x</param>
	/// <param name="EndAngle">�ڕW�̊p�x</param>
	static float RotateEarliestArc(float NowAngle, float EndAngle) {
		//�p�x�̍���180�𒴂��Ă���ꍇ�ɍŒZ�̌ʂ����߂�
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
	/// �p�x��0�`360�Ɋۂ߂�
	/// </summary>
	/// <param name="Angle">�p�x</param>
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
	/// ���W�A����x�ɕϊ�
	/// </summary>
	/// <param name="radian">���W�A��</param>
	static double degrees(double radian) {
		//����180 / �΁~��
		return (180.0 / M_PI) * radian;
	}

	/// <summary>
	/// �x�����W�A���ɕϊ�
	/// </summary>
	/// <param name="radian">���W�A��</param>
	static double radian(double radian) {
		//������ / 180�~��
		return (M_PI / 180.0) * radian;
	}


	// 2�̐��l��atan2�Ń��W�A���ɂ��Ċp�x�ɕϊ�(�������0�x)
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
		// x���W�̍����v�Z����dx�ɑ��
		dx = x2 - x1;
		// y���W�̍����v�Z����dy�ɑ��
		dy = y2 - y1;
		// y���W�̍����v�Z����dy�ɑ��
		dz = z2 - z1;
		
		l = sqrt(dx * dx + dy * dy + dz * dz);
		
		return l;
	}

	//�_���m�̋��������߂�
	static double distance_XMF3(const DirectX::XMFLOAT3 p1, const DirectX::XMFLOAT3 p2)
	{
		double l;
		double dx, dy, dz;
		// x���W�̍����v�Z����dx�ɑ��
		dx = (double)p2.x - (double)p1.x;
		// y���W�̍����v�Z����dy�ɑ��
		dy = (double)p2.y - (double)p1.y;
		// y���W�̍����v�Z����dy�ɑ��
		dz = (double)p2.z - (double)p1.z;

		l = sqrt(dx * dx + dy * dy + dz * dz);

		return l;
	}

	//�l��͈͓��Ɏ��߂� [low, high]
	static float clamp(float value, float low, float high) {
		value = (value < low) ? low : value;
		value = (value > high) ? high : value;
		return value;
	}

	/// <summary>
	/// �l�̎w��ł͈̔͂�0�`1�̊��������߂�
	/// </summary>
	/// <param name="Value">�l</param>
	/// <param name="low">����</param>
	/// <param name="high">���</param>
	/// <returns></returns>
	inline float CalPercentageZeroToOne(const float &value, const float &low, const float &high) {
		float max = high;
		float now = value;
		//�ŏ��l��0�����̏ꍇ�͍ő�l�ɑ���
		if (low < 0.0f) {
			max += fabs(low);
			now += fabs(low);
		}
		//�ŏ��l��0���傫���ꍇ�͍ő�l�������
		else if (low > 0.0f) {
			max -= fabs(low);
			now -= fabs(low);
		}

		float result = now / max;

		//0�ȉ�1�ȏ�ɂȂ�Ȃ��悤�ɂ���
		if (result <= 0.0f) result = 0.0f;
		if (result >= 1.0f) result = 1.0f;

		return result;

	}

	/// <summary>
	/// �p�x��P�ʕ����x�N�g���ɕϊ�
	/// </summary>
	/// <param name="Angle">�p�x</param>
	/// <returns></returns>
	static DirectX::XMFLOAT2 AngleToDirectionVector(float Angle) {
		float Radian = (float)radian(Angle);
		DirectX::XMFLOAT2 result = { cosf(Radian),sinf(Radian) };
		return result;
	}

	/// <summary>
	/// pos1��pos1front�̕����������Ă��鎞��pos2�����E�ǂ���ɋ��邩�����߂�
	/// false �Ȃ� ��  :  true �Ȃ� �E
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
	/// �����x�N�g������]������
	/// </summary>
	/// <param name="dir">�����x�N�g��</param>
	/// <param name="rotate">��]�p�x</param>
	/// <returns></returns>
	static DirectX::XMFLOAT2 RotateVector(DirectX::XMFLOAT2 dir, float rotate) {
		float rot = rotate * (M_PI_F / 180.0f);
		float rx = cosf(rot) * dir.x - sinf(rot) * dir.y;
		float ry = sinf(rot) * dir.x + cosf(rot) * dir.y;
		return  DirectX::XMFLOAT2(rx, ry);
	}

	/// <summary>
	/// value��m���ڂ���n���ڂ��擾����
	/// </summary>
	/// <param name="value">�擾��ƂȂ镂�������_��</param>
	/// <param name="m">�擾���J�n���錅</param>
	/// <param name="n">�擾���I�����錅</param>
	/// <returns></returns>
	static int GetDigits(int value, int m, int n) {
		double mod_value;
		int result;
		// ��Βl�����߂�
		value = int(fabs(value));
		// n���ڈȉ��̌����擾
		mod_value = fmod(value, pow(10, n + 1));
		// m���ڈȏ�̌����擾
		result = int(mod_value / pow(10, m));

		return result;
	}
}