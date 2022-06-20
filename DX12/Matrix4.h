#pragma once
#include "Vector3.h"
#include <math.h>

struct MATRIX {
	float m[4][4] = {0};
};

struct Matrix4 : MATRIX {
	Matrix4() {};
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33){
		m[0][0] = m00; m[0][1] = m01; m[0][1] = m02; m[0][1] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][1] = m12; m[1][1] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][1] = m22; m[2][1] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][1] = m32; m[3][1] = m33;
	}

	Matrix4 operator+() {
		return *this;
	}
	Matrix4 operator-() {
		Matrix4 other;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				other.m[y][x] = -m[y][x];
			}
		}
		return other;
	}
	Matrix4 &operator=(const Matrix4 &other) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				this->m[y][x] = other.m[y][x];
			}
		}
		return *this;
	}
	Matrix4 operator+=(const Matrix4 &other) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				m[y][x] += other.m[y][x];
			}
		}
		return *this;
	}
	Matrix4 operator-=(const Matrix4 &other) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				m[y][x] -= other.m[y][x];
			}
		}
		return *this;
	}
	Matrix4 operator*=(const float value) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				this->m[y][x] *= value;
			}
		}
		return *this;
	}
	Matrix4 operator/=(const float value) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				this->m[y][x] /= value;
			}
		}
	}
	bool operator==(const Matrix4 &other) const {
		bool result = true;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				result &= (m[y][x] == other.m[y][x]);
			}
		}
		return result;
	}
	bool operator!=(const Matrix4 &other) const {
		return !((*this) == other);
	}
};

inline Matrix4 operator+(const Matrix4 &A, const Matrix4 &B) {
	Matrix4 Result;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			Result.m[y][x] = A.m[y][x] + B.m[y][x];
		}
	}
	return Result;
}
inline Matrix4 operator-(const Matrix4 &A, const Matrix4 &B) {
	Matrix4 Result;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			Result.m[y][x] = A.m[y][x] - B.m[y][x];
		}
	}
	return Result;
}
inline Matrix4 operator*(const Matrix4 &A, const Matrix4 &B) {
	Matrix4 Result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				Result.m[i][j] += A.m[i][k] * B.m[k][j];
			}
		}
	}
	return Result;
}
inline Matrix4 operator/(const Matrix4 &A, const Matrix4 &B) {
	Matrix4 Result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				Result.m[i][j] += A.m[i][k] / B.m[k][j];
			}
		}
	}
	return Result;
}


//‹ts—ñ
static inline float CalDetMat4x4_A(const Matrix4 &M) {
	float m[16] = {
		M.m[0][0], M.m[0][1], M.m[0][2], M.m[0][3],
		M.m[1][0], M.m[1][1], M.m[1][2], M.m[1][3],
		M.m[2][0], M.m[2][1], M.m[2][2], M.m[2][3],
		M.m[3][0], M.m[3][1], M.m[3][2], M.m[3][3]
	};
	return m[0] * m[5] * m[10] * m[15] + m[0] * m[6] * m[11] * m[13] + m[0] * m[7] * m[9] * m[14]
		+ m[1] * m[4] * m[11] * m[14] + m[1] * m[6] * m[8] * m[15] + m[1] * m[7] * m[10] * m[12]
		+ m[2] * m[4] * m[9] * m[15] + m[2] * m[5] * m[11] * m[12] + m[2] * m[7] * m[8] * m[13]
		+ m[3] * m[4] * m[10] * m[13] + m[3] * m[5] * m[8] * m[14] + m[3] * m[6] * m[9] * m[12]
		- m[0] * m[5] * m[11] * m[14] - m[0] * m[6] * m[9] * m[15] - m[0] * m[7] * m[10] * m[13]
		- m[1] * m[4] * m[10] * m[15] - m[1] * m[6] * m[11] * m[12] - m[1] * m[7] * m[8] * m[14]
		- m[2] * m[4] * m[11] * m[13] - m[2] * m[5] * m[8] * m[15] - m[2] * m[7] * m[9] * m[12]
		- m[3] * m[4] * m[9] * m[14] - m[3] * m[5] * m[10] * m[12] - m[3] * m[6] * m[8] * m[13];
}
static inline Matrix4 ClaDetMat4x4(const Matrix4 &M) {
	float m[16] = {
		M.m[0][0], M.m[0][1], M.m[0][2], M.m[0][3],
		M.m[1][0], M.m[1][1], M.m[1][2], M.m[1][3],
		M.m[2][0], M.m[2][1], M.m[2][2], M.m[2][3],
		M.m[3][0], M.m[3][1], M.m[3][2], M.m[3][3]
	};

	float det = CalDetMat4x4_A(M);

	float inv_det = 1.0f / det;

	Matrix4 invm;

	invm.m[0][0] = inv_det * (m[5] * m[10] * m[15] + m[6] * m[11] * m[13] + m[7] * m[9] * m[14] - m[5] * m[11] * m[14] - m[6] * m[9] * m[15] - m[7] * m[10] * m[13]);
	invm.m[0][1] = inv_det * (m[1] * m[11] * m[14] + m[2] * m[9] * m[15] + m[3] * m[10] * m[13] - m[1] * m[10] * m[15] - m[2] * m[11] * m[13] - m[3] * m[9] * m[14]);
	invm.m[0][2] = inv_det * (m[1] * m[6] * m[15] + m[2] * m[7] * m[13] + m[3] * m[5] * m[14] - m[1] * m[7] * m[14] - m[2] * m[5] * m[15] - m[3] * m[6] * m[13]);
	invm.m[0][3] = inv_det * (m[1] * m[7] * m[10] + m[2] * m[5] * m[11] + m[3] * m[6] * m[9] - m[1] * m[6] * m[11] - m[2] * m[7] * m[9] - m[3] * m[5] * m[10]);

	invm.m[1][0] = inv_det * (m[4] * m[11] * m[14] + m[6] * m[8] * m[15] + m[7] * m[10] * m[12] - m[4] * m[10] * m[15] - m[6] * m[11] * m[12] - m[7] * m[8] * m[14]);
	invm.m[1][1] = inv_det * (m[0] * m[10] * m[15] + m[2] * m[11] * m[12] + m[3] * m[8] * m[14] - m[0] * m[11] * m[14] - m[2] * m[8] * m[15] - m[3] * m[10] * m[12]);
	invm.m[1][2] = inv_det * (m[0] * m[7] * m[14] + m[2] * m[4] * m[15] + m[3] * m[6] * m[12] - m[0] * m[6] * m[15] - m[2] * m[7] * m[12] - m[3] * m[4] * m[14]);
	invm.m[1][3] = inv_det * (m[0] * m[6] * m[11] + m[2] * m[7] * m[8] + m[3] * m[4] * m[10] - m[0] * m[7] * m[10] - m[2] * m[4] * m[11] - m[3] * m[6] * m[8]);

	invm.m[2][0] = inv_det * (m[4] * m[9] * m[15] + m[5] * m[11] * m[12] + m[7] * m[8] * m[13] - m[4] * m[11] * m[13] - m[5] * m[8] * m[15] - m[7] * m[9] * m[12]);
	invm.m[2][1] = inv_det * (m[0] * m[11] * m[13] + m[1] * m[8] * m[15] + m[3] * m[9] * m[12] - m[0] * m[9] * m[15] - m[1] * m[11] * m[12] - m[3] * m[8] * m[13]);
	invm.m[2][2] = inv_det * (m[0] * m[5] * m[15] + m[1] * m[7] * m[12] + m[3] * m[4] * m[13] - m[0] * m[7] * m[13] - m[1] * m[4] * m[15] - m[3] * m[5] * m[12]);
	invm.m[2][3] = inv_det * (m[0] * m[7] * m[9] + m[1] * m[4] * m[11] + m[3] * m[5] * m[8] - m[0] * m[5] * m[11] - m[1] * m[7] * m[8] - m[3] * m[4] * m[9]);

	invm.m[3][0] = inv_det * (m[4] * m[10] * m[13] + m[5] * m[8] * m[14] + m[6] * m[9] * m[12] - m[4] * m[9] * m[14] - m[5] * m[10] * m[12] - m[6] * m[8] * m[13]);
	invm.m[3][1] = inv_det * (m[0] * m[9] * m[14] + m[1] * m[10] * m[12] + m[2] * m[8] * m[13] - m[0] * m[10] * m[13] - m[1] * m[8] * m[14] - m[2] * m[9] * m[12]);
	invm.m[3][2] = inv_det * (m[0] * m[6] * m[13] + m[1] * m[4] * m[14] + m[2] * m[5] * m[12] - m[0] * m[5] * m[14] - m[1] * m[6] * m[12] - m[2] * m[4] * m[13]);
	invm.m[3][3] = inv_det * (m[0] * m[5] * m[10] + m[1] * m[6] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] - m[2] * m[5] * m[8]);

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (isnan(invm.m[y][x]) == true) {
				Matrix4 NULLmat;
				return NULLmat;
			}
		}
	}

	return invm;
}

static inline Matrix4 idebtity() {
	static const Matrix4 Result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return Result;
}
static inline Matrix4 scale(const Vector3 &Size) {
	Matrix4 Result
	{
		Size.x,0.0f,0.0f,0.0f,
		0.0f,Size.y,0.0f,0.0f,
		0.0f,0.0f,Size.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return Result;
}
static inline Matrix4 rotateX(float Angle) {
	float Sin = sinf(Angle);
	float Cos = cosf(Angle);
	Matrix4 Result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,Cos,Sin,0.0f,
		0.0f,-Sin,Cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return Result;
}
static inline Matrix4 rotateY(float Angle) {
	float Sin = sinf(Angle);
	float Cos = cosf(Angle);
	Matrix4 Result
	{
		Cos,0.0f,-Sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		Sin,0.0f,Cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return Result;
}
static inline Matrix4 rotateZ(float Angle) {
	float Sin = sinf(Angle);
	float Cos = cosf(Angle);
	Matrix4 Result
	{
		Cos,Sin,0.0f,0.0f,
		-Sin,Cos,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return Result;
}
static inline Matrix4 translate(const Vector3 &offset) {
	Matrix4 Result
	{
		1.0f,0.0f,0.0f,offset.x,
		0.0f,1.0f,0.0f,offset.y,
		0.0f,0.0f,1.0f,offset.z,
		0.0f,0.0f,0.0f,1.0f
	};
	return Result;
}
static inline Vector3 transform(const Vector3 &VOffset, const Matrix4 &MOffset) {
	float hoge = VOffset.x * MOffset.m[0][3] + VOffset.y * MOffset.m[1][3] + VOffset.z * MOffset.m[2][3] + MOffset.m[3][3];
	Vector3 Result
	{
		(VOffset.x * MOffset.m[0][0] + VOffset.y * MOffset.m[1][0] + VOffset.z * MOffset.m[2][0] + MOffset.m[3][0]) / hoge,
		(VOffset.x * MOffset.m[0][1] + VOffset.y * MOffset.m[1][1] + VOffset.z * MOffset.m[2][1] + MOffset.m[3][1]) / hoge,
		(VOffset.x * MOffset.m[0][2] + VOffset.y * MOffset.m[1][2] + VOffset.z * MOffset.m[2][2] + MOffset.m[3][2]) / hoge
	};
	return Result;
}
static inline Vector3 operator*(const Vector3 &VOffset, const Matrix4 &MOffset) {
	return transform(VOffset, MOffset);
}