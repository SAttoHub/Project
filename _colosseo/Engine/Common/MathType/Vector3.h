#pragma once
#include "math.h"

//typedef struct VECTOR {
//	float x, y, z;
//} FLOAT3;
//
//struct Vector3 : public VECTOR {
//	Vector3(float x, float y, float z) : VECTOR{ x, y, z } {}
//	Vector3() : VECTOR{ 0, 0, 0 } {}
//	Vector3 operator+(const Vector3 &other)const { return Vector3(x + other.x, y + other.y, z + other.z); }
//	Vector3 operator-(const Vector3 &other)const { return Vector3(x - other.x, y - other.y, z - other.z); }
//	Vector3 operator*(const Vector3 &other)const { return Vector3(x * other.x, y * other.y, z * other.z); }
//	Vector3 operator/(const Vector3 &other)const { return Vector3(x / other.x, y / other.y, z / other.z); }
//	Vector3 &operator+=(const Vector3 &other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
//	Vector3 &operator-=(const Vector3 &other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
//	Vector3 &operator*=(const Vector3 &other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
//	Vector3 &operator/=(const Vector3 &other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }
//	bool operator==(const Vector3 &other) { return this->x == other.x && this->y == other.y && this->z == other.z; }
//	bool operator!=(const Vector3 &other) { return !(*this == other); }
//
//	Vector3 operator*(const float &other) { return Vector3(x * other, y * other, z * other); }
//	Vector3 operator/(const float &other) { return Vector3(x / other, y / other, z / other); }
//	Vector3 operator*=(const float &other) { this->x *= other; this->y *= other; return *this; }
//	Vector3 operator/=(const float &other) { this->x /= other; this->y /= other; return *this; }
//
//	float length() { return sqrtf(x * x + y * y + z * z); }
//	Vector3 norm() { return Vector3(x / length(), y / length(), z / length()); }
//
//	Vector3 operator-() { return Vector3(-x, -y, -z); }
//};

//static inline float dot(const Vector3 &a, const Vector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
//static inline Vector3 cross(const Vector3 &a, const Vector3 &b) { return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
//
//static inline Vector3 operator*(const float &sum, const Vector3 &sum2) { return Vector3(sum2.x * sum, sum2.y * sum, sum2.z * sum); }
//static inline Vector3 operator/(const float &sum, const Vector3 &sum2) { return Vector3(sum2.x / sum, sum2.y / sum, sum2.z / sum); }