#pragma once
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <DirectXMath.h>
#include <random>
#include "..\..\Singleton.h"
#include "SDifines.h"

//-------------------------------------------------------------------------------------------------------------
// ���������p
//-------------------------------------------------------------------------------------------------------------
class RandomFactory : public Singleton<RandomFactory> {
	friend class Singleton<RandomFactory>;
private:
	std::random_device randomDevice;
	std::mt19937 *mt; //�f�t�H���g�V�[�h
private:
	//�ŏ��ƍő���\�[�g����
	template <typename T>
	void ShiftValue(T *min, T *max) {
		//�ꎞ�I�ɒl��ۑ�
		T _min = *min, _max = *max;
		if (_min < _max) {
			*min = _max;
			*max = _min;
		}
	}
public:
	RandomFactory() {
		mt = new std::mt19937(randomDevice());
	};
	~RandomFactory() {
		delete mt;
	};

	void ChangeSeed(unsigned int Seed) {
		delete mt;
		mt = new std::mt19937(Seed);
	}

	std::mt19937* GetMT() { return mt; }

	int GetRand(const int &min, const int &max) {
		//�����̑傫�����擾
		int RandMax = max, RandMin = min;
		ShiftValue(&RandMax, &RandMin);
		//�������擾
		std::uniform_int_distribution<> distr(RandMin, RandMax);
		return distr(*mt);
	}
	float GetRand(const float &min, const float &max) {
		//�����̑傫�����擾
		float RandMax = max, RandMin = min;
		ShiftValue(&RandMax, &RandMin);
		//�������擾
		std::uniform_real_distribution<float> distr(RandMin, RandMax);
		return distr(*mt);
	}
	double GetRand(const double &min, const double &max) {
		//�����̑傫�����擾
		double RandMax = max, RandMin = min;
		ShiftValue(&RandMax, &RandMin);
		//�������擾
		std::uniform_real_distribution<double> distr(RandMin, RandMax);
		return distr(*mt);
	}
};

//-------------------------------------------------------------------------------------------------------------
// ��{�����擾�֐�
//-------------------------------------------------------------------------------------------------------------

//�������擾����
static int GetRand(const int &min, const int &max) {
	return RandomFactory::Instance()->GetRand(min, max);
}
static float GetRand(const float &min, const float &max) {
	return RandomFactory::Instance()->GetRand(min, max);
}
static double GetRand(const double &min, const double &max) {
	return RandomFactory::Instance()->GetRand(min, max);
}

//�������擾���� [ XMINT2 ]
static DirectX::XMINT2 GetRandXMFLOAT2(const int &min, const int &max) {
	return DirectX::XMINT2(GetRand(min, max), GetRand(min, max));
}
//�������擾���� [ XMINT2 ]
static DirectX::XMINT2 GetRandXMFLOAT2(const DirectX::XMINT2 &min, const DirectX::XMINT2 &max) {
	return DirectX::XMINT2(GetRand(min.x, max.x), GetRand(min.y, max.y));
}
//�������擾���� [ XMINT3 ]
static DirectX::XMINT3 GetRandXMFLOAT3(const int &min, const int &max) {
	return DirectX::XMINT3(GetRand(min, max), GetRand(min, max), GetRand(min, max));
}
//�������擾���� [ XMINT3 ]
static DirectX::XMINT3 GetRandXMFLOAT3(const DirectX::XMINT3 &min, const DirectX::XMINT3 &max) {
	return DirectX::XMINT3(GetRand(min.x, max.x), GetRand(min.y, max.y), GetRand(min.z, max.z));
}
//�������擾���� [ XMINT4 ]
static DirectX::XMINT4 GetRandXMFLOAT4(const int &min, const int &max) {
	return DirectX::XMINT4(GetRand(min, max), GetRand(min, max), GetRand(min, max), GetRand(min, max));
}
//�������擾���� [ XMINT4 ]
static DirectX::XMINT4 GetRandXMFLOAT4(const DirectX::XMINT4 &min, const DirectX::XMINT4 &max) {
	return DirectX::XMINT4(GetRand(min.x, max.x), GetRand(min.y, max.y), GetRand(min.z, max.z), GetRand(min.w, max.w));
}
//�������擾���� [ XMFLOAT2 ]
static DirectX::XMFLOAT2 GetRandXMFLOAT2(const float &min, const float &max) {
	return DirectX::XMFLOAT2(GetRand(min, max), GetRand(min, max));
}
//�������擾���� [ XMFLOAT2 ]
static DirectX::XMFLOAT2 GetRandXMFLOAT2(const DirectX::XMFLOAT2 &min, const DirectX::XMFLOAT2 &max) {
	return DirectX::XMFLOAT2(GetRand(min.x, max.x), GetRand(min.y, max.y));
}
//�������擾���� [ XMFLOAT3 ]
static DirectX::XMFLOAT3 GetRandXMFLOAT3(const float &min, const float &max) {
	return DirectX::XMFLOAT3(GetRand(min, max), GetRand(min, max), GetRand(min, max));
}
//�������擾���� [ XMFLOAT3 ]
static DirectX::XMFLOAT3 GetRandXMFLOAT3(const DirectX::XMFLOAT3 &min, const DirectX::XMFLOAT3 &max) {
	return DirectX::XMFLOAT3(GetRand(min.x, max.x), GetRand(min.y, max.y), GetRand(min.z, max.z));
}
//�������擾���� [ XMFLOAT4 ]
static DirectX::XMFLOAT4 GetRandXMFLOAT4(const float &min, const float &max) {
	return DirectX::XMFLOAT4(GetRand(min, max), GetRand(min, max), GetRand(min, max), GetRand(min, max));
}
//�������擾���� [ XMFLOAT4 ]
static DirectX::XMFLOAT4 GetRandXMFLOAT4(const DirectX::XMFLOAT4 &min, const DirectX::XMFLOAT4 &max) {
	return DirectX::XMFLOAT4(GetRand(min.x, max.x), GetRand(min.y, max.y), GetRand(min.z, max.z), GetRand(min.w, max.w));
}

//-------------------------------------------------------------------------------------------------------------
// ���W�����擾�֐�
//-------------------------------------------------------------------------------------------------------------

/// <summary>
/// �w��͈͂����O�������������߂�
/// </summary>
/// <param name="min">�ŏ�</param>
/// <param name="max">�ő�</param>
/// <param name="ExcRangeMin">���O�͈͂̍ŏ�</param>
/// <param name="ExcRangeMax">���O�͈͂̍ő�</param>
/// <returns></returns>
static int GetRandRangeExc(const int &min, const int &max, const int &ExcRangeMin, const int &ExcRangeMax) {
	int Result1 = GetRand(min, ExcRangeMin);
	int Result2 = GetRand(ExcRangeMax, max);
	int Choice = GetRand(0, 1);
	if (Choice == 0) {
		return Result1;
	}
	else {
		return Result2;
	}
	return 0;
}
/// <summary>
/// �w��͈͂����O�������������߂�
/// </summary>
/// <param name="min">�ŏ�</param>
/// <param name="max">�ő�</param>
/// <param name="ExcRangeMin">���O�͈͂̍ŏ�</param>
/// <param name="ExcRangeMax">���O�͈͂̍ő�</param>
/// <returns></returns>
static float GetRandRangeExc(const float &min, const float &max, const float &ExcRangeMin, const float &ExcRangeMax) {
	float Result1 = GetRand(min, ExcRangeMin);
	float Result2 = GetRand(ExcRangeMax, max);
	float Choice = GetRand(0.0f, 1.0f);
	if (Choice == 0) {
		return Result1;
	}
	else {
		return Result2;
	}
	return 0;
}
/// <summary>
/// �w��͈͂����O�������������߂�
/// </summary>
/// <param name="min">�ŏ�</param>
/// <param name="max">�ő�</param>
/// <param name="ExcRangeMin">���O�͈͂̍ŏ�</param>
/// <param name="ExcRangeMax">���O�͈͂̍ő�</param>
/// <returns></returns>
static double GetRandRangeExc(const double &min, const double &max, const double &ExcRangeMin, const double &ExcRangeMax) {
	double Result1 = GetRand(min, ExcRangeMin);
	double Result2 = GetRand(ExcRangeMax, max);
	double Choice = GetRand(0.0, 1.0);
	if (Choice == 0) {
		return Result1;
	}
	else {
		return Result2;
	}
	return 0;
}

/// <summary>
/// �~��ɗ����𐶐��B�ψꉻ����(���S�ɏW�܂�₷��)
/// </summary>
/// <param name="range">���a</param>
/// <returns></returns>
static DirectX::XMFLOAT2 GetRandInCircle(float range) {
	float Range = GetRand(0.0f, 1.0f);
	float Theta = GetRand(-M_PI_F, M_PI_F);
	DirectX::XMFLOAT2 result;
	result.x = Range * cosf(Theta);
	result.y = Range * sinf(Theta);
	return result;
}

/// <summary>
/// �~��ɗ����𐶐��B�ψꉻ�L��
/// </summary>
/// <param name="range">���a</param>
/// <returns></returns>
static DirectX::XMFLOAT2 GetRandInUniformInCircle(float range) {
	float Range = sqrtf(GetRand(0.0f, 1.0f));
	float Theta = GetRand(-M_PI_F, M_PI_F);
	DirectX::XMFLOAT2 result;
	result.x = Range * cosf(Theta);
	result.y = Range * sinf(Theta);
	return result;
}

/// <summary>
/// ���̕\�ʂɉ����悤�ɗ����𐶐�
/// </summary>
/// <param name="range">���a</param>
/// <returns></returns>
static DirectX::XMFLOAT3 GetRandSurfaceOfSphere(float range) {
	float Theta = GetRand(-M_PI_F, M_PI_F);
	float P = sqrtf(GetRand(0.0f, 1.0f));
	float phi = asinf((2.0f * P) - 1.0f);
	DirectX::XMFLOAT3 result;
	result.x = cosf(phi) * cosf(Theta);
	result.y = cosf(phi) * sinf(Theta);
	result.z = sinf(phi);
	return result;
}

/// <summary>
/// �����̕\�ʂɉ����悤�ɗ����𐶐�
/// </summary>
/// <param name="range">���a</param>
/// <returns></returns>
static DirectX::XMFLOAT3 GetRandSurfaceOfHemiSphere(float range) {
	float Theta = GetRand(-M_PI_F, M_PI_F);
	float P = sqrtf(GetRand(0.0f, 1.0f));
	float phi = asinf(P);
	DirectX::XMFLOAT3 result;
	result.x = cosf(phi) * cosf(Theta);
	result.y = cosf(phi) * sinf(Theta);
	result.z = sinf(phi);
	return result;
}

/// <summary>
/// ���̕\�ʂɉ����悤�Ɂ@���@�ɂɊ񂹂�͂��w�肵�ā@�����𐶐�
/// </summary>
/// <param name="range">���a</param>
/// <param name="Power">�񂹂��</param>
/// <returns></returns>
static DirectX::XMFLOAT3 GetRandSurfaceOfHemiSpherePole(float range, float Power) {
	float Theta = GetRand(-M_PI_F, M_PI_F);
	float P = sqrtf(GetRand(0.0f, 1.0f));
	float phi = asinf(pow(P, 1.0f / (Power + 1.0f)));
	DirectX::XMFLOAT3 result;
	result.x = cosf(phi) * cosf(Theta);
	result.y = cosf(phi) * sinf(Theta);
	result.z = sinf(phi);
	return result;
}