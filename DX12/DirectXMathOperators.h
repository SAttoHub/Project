#pragma once
#include <DirectXMath.h>
#include <math.h>
#include <cassert>
#include <limits>

//-------------------------------------------------------------------------------------------------------------
//
// DirectXMath operatorä÷òA
//
//-------------------------------------------------------------------------------------------------------------

//å„Ç≈ïÇìÆè¨êîì_ÇÃåÎç∑Ç…óßÇøå¸Ç©Ç¢ÇΩÇ¢
template<typename T>
static inline constexpr bool AdjEqual(const T num1, const T num2)
{
    return (num1 == num2);
}


static inline int32_t tmodt(int32_t x, int32_t y)
{
    float result;
    result = (float)remainder(fabs((float)x), (y = (int32_t)(fabs((float)y))));
    if (signbit(result)) result += (float)y;
    return (int32_t)copysign(result, (float)x);
}

static inline uint32_t utmodut(uint32_t x, uint32_t y)
{
    float result;
    result = (float)remainder(fabs((float)x), (y = (uint32_t)(fabs((float)y))));
    if (signbit(result)) result += (float)y;
    return (uint32_t)copysign(result, (float)x);
}

//-------------------------------------------------------------------------------------------------------------
// XMFLOAT4ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMFLOAT4

static inline void operator+= (DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;
}

static inline void operator-= (DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;
}

static inline void operator/= (DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
    v1.w /= v2.w;
}

static inline void operator*= (DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    v1.w *= v2.w;
}

static inline void operator%= (DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    v1.x = ::fmodf(v1.x, v2.x);
    v1.y = ::fmodf(v1.y, v2.y);
    v1.z = ::fmodf(v1.z, v2.z);
    v1.w = ::fmodf(v1.w, v2.w);
}

static inline constexpr auto operator+ (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return DirectX::XMFLOAT4{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

static inline constexpr auto operator- (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return DirectX::XMFLOAT4{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

static inline constexpr auto operator* (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return DirectX::XMFLOAT4{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

static inline constexpr auto operator/ (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return DirectX::XMFLOAT4{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}

static inline constexpr auto operator% (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return DirectX::XMFLOAT4{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y), ::fmodf(v1.z, v2.z), ::fmodf(v1.w, v2.w) };
}

static inline constexpr auto operator+ (const DirectX::XMFLOAT4 &v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x + num, v1.y + num, v1.z + num, v1.w + num };
}

static inline constexpr auto operator- (const DirectX::XMFLOAT4 &v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x - num, v1.y - num, v1.z - num, v1.w - num };
}

static inline constexpr auto operator* (const DirectX::XMFLOAT4 &v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x * num, v1.y * num, v1.z * num, v1.w * num };
}

static inline constexpr auto operator/ (const DirectX::XMFLOAT4 &v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x / num, v1.y / num, v1.z / num, v1.w / num };
}

static inline constexpr auto operator% (const DirectX::XMFLOAT4 &v1, const float num)
{
    return DirectX::XMFLOAT4{ ::fmodf(v1.x, num), ::fmodf(v1.y, num), ::fmodf(v1.z, num), ::fmodf(v1.w, num) };
}

static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT4 &v1)
{
    return DirectX::XMFLOAT4{ num + v1.x, num + v1.y, num + v1.z, num + v1.w };
}

static inline constexpr auto operator- (const float num, DirectX::XMFLOAT4 &v1)
{
    return DirectX::XMFLOAT4{ num - v1.x, num - v1.y, num - v1.z, num - v1.w };
}

static inline constexpr auto operator* (const float num, DirectX::XMFLOAT4 &v1)
{
    return DirectX::XMFLOAT4{ num * v1.x, num * v1.y, num * v1.z, num * v1.w };
}

static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT4 &v1)
{
    return DirectX::XMFLOAT4{ num / v1.x, num / v1.y, num / v1.z, num / v1.w };
}

static inline constexpr auto operator% (const float num, DirectX::XMFLOAT4 &v1)
{
    return DirectX::XMFLOAT4{ fmodf(num, v1.x), fmodf(num, v1.y), fmodf(num, v1.z), fmodf(num, v1.w) };
}

static inline constexpr bool operator== (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator!= (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator== (const DirectX::XMFLOAT4 &v1, const float num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

static inline constexpr bool operator!= (const DirectX::XMFLOAT4 &v1, const float num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

static inline constexpr bool operator< (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w));
}

static inline constexpr bool operator> (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w));
}

static inline constexpr bool operator<= (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator>= (const DirectX::XMFLOAT4 &v1, const DirectX::XMFLOAT4 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr auto operator- (const DirectX::XMFLOAT4 &v1)
{
    return DirectX::XMFLOAT4{ -v1.x, -v1.y, -v1.z, -v1.w };
}

#pragma endregion
//-------------------------------------------------------------------------------------------------------------
// XMFLOAT3ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMFLOAT3

static inline void operator+= (DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
}

static inline void operator-= (DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
}

static inline void operator/= (DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
}

static inline void operator*= (DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
}

static inline void operator%= (DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    v1.x = ::fmodf(v1.x, v2.x);
    v1.y = ::fmodf(v1.y, v2.y);
    v1.z = ::fmodf(v1.z, v2.z);
}

static inline constexpr auto operator+ (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return DirectX::XMFLOAT3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

static inline constexpr auto operator- (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return DirectX::XMFLOAT3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

static inline constexpr auto operator* (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return DirectX::XMFLOAT3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

static inline constexpr auto operator/ (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return DirectX::XMFLOAT3{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

static inline constexpr auto operator% (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return DirectX::XMFLOAT3{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y), ::fmodf(v1.z, v2.z) };
}

static inline constexpr auto operator+ (const DirectX::XMFLOAT3 &v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x + num, v1.y + num, v1.z + num };
}

static inline constexpr auto operator- (const DirectX::XMFLOAT3 &v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x - num, v1.y - num, v1.z - num };
}

static inline constexpr auto operator* (const DirectX::XMFLOAT3 &v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x * num, v1.y * num, v1.z * num };
}

static inline constexpr auto operator/ (const DirectX::XMFLOAT3 &v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x / num, v1.y / num, v1.z / num };
}

static inline constexpr auto operator% (const DirectX::XMFLOAT3 &v1, const float num)
{
    return DirectX::XMFLOAT3{ ::fmodf(v1.x, num), ::fmodf(v1.y, num), ::fmodf(v1.z, num) };
}

static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT3 &v1)
{
    return DirectX::XMFLOAT3{ num + v1.x, num + v1.y, num + v1.z };
}

static inline constexpr auto operator- (const float num, DirectX::XMFLOAT3 &v1)
{
    return DirectX::XMFLOAT3{ num - v1.x, num - v1.y, num - v1.z };
}

static inline constexpr auto operator* (const float num, DirectX::XMFLOAT3 &v1)
{
    return DirectX::XMFLOAT3{ num * v1.x, num * v1.y, num * v1.z };
}

static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT3 &v1)
{
    return DirectX::XMFLOAT3{ num / v1.x, num / v1.y, num / v1.z };
}

static inline constexpr auto operator% (const float num, DirectX::XMFLOAT3 &v1)
{
    return DirectX::XMFLOAT3{ fmodf(num, v1.x), fmodf(num, v1.y), fmodf(num, v1.z) };
}

static inline constexpr bool operator== (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator!= (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator== (const DirectX::XMFLOAT3 &v1, const float num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

static inline constexpr bool operator!= (const DirectX::XMFLOAT3 &v1, const float num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

static inline constexpr bool operator< (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z));
}

static inline constexpr bool operator> (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z));
}

static inline constexpr bool operator<= (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator>= (const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr auto operator- (const DirectX::XMFLOAT3 &v1)
{
    return DirectX::XMFLOAT3{ -v1.x, -v1.y, -v1.z };
}

#pragma endregion
//-------------------------------------------------------------------------------------------------------------
// XMFLOAT2ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMFLOAT2

static inline void operator+= (DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
}

static inline void operator-= (DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
}

static inline void operator/= (DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
}

static inline void operator*= (DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
}

static inline void operator%= (DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    v1.x = ::fmodf(v1.x, v2.x);
    v1.y = ::fmodf(v1.y, v2.y);
}

static inline constexpr auto operator+ (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return DirectX::XMFLOAT2{ v1.x + v2.x, v1.y + v2.y };
}

static inline constexpr auto operator- (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return DirectX::XMFLOAT2{ v1.x - v2.x, v1.y - v2.y };
}

static inline constexpr auto operator* (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return DirectX::XMFLOAT2{ v1.x * v2.x, v1.y * v2.y };
}

static inline constexpr auto operator/ (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return DirectX::XMFLOAT2{ v1.x / v2.x, v1.y / v2.y };
}

static inline constexpr auto operator% (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return DirectX::XMFLOAT2{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y) };
}

static inline constexpr auto operator+ (const DirectX::XMFLOAT2 &v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x + num, v1.y + num };
}

static inline constexpr auto operator- (const DirectX::XMFLOAT2 &v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x - num, v1.y - num };
}

static inline constexpr auto operator* (const DirectX::XMFLOAT2 &v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x * num, v1.y * num };
}

static inline constexpr auto operator/ (const DirectX::XMFLOAT2 &v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x / num, v1.y / num };
}

static inline constexpr auto operator% (const DirectX::XMFLOAT2 &v1, const float num)
{
    return DirectX::XMFLOAT2{ ::fmodf(v1.x, num), ::fmodf(v1.y, num) };
}

static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT2 &v1)
{
    return DirectX::XMFLOAT2{ num + v1.x, num + v1.y };
}

static inline constexpr auto operator- (const float num, DirectX::XMFLOAT2 &v1)
{
    return DirectX::XMFLOAT2{ num - v1.x, num - v1.y };
}

static inline constexpr auto operator* (const float num, DirectX::XMFLOAT2 &v1)
{
    return DirectX::XMFLOAT2{ num * v1.x, num * v1.y };
}

static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT2 &v1)
{
    return DirectX::XMFLOAT2{ num / v1.x, num / v1.y };
}

static inline constexpr auto operator% (const float num, DirectX::XMFLOAT2 &v1)
{
    return DirectX::XMFLOAT2{ fmodf(num, v1.x), fmodf(num, v1.y) };
}

static inline constexpr bool operator== (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator!= (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator== (const DirectX::XMFLOAT2 &v1, const float num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

static inline constexpr bool operator!= (const DirectX::XMFLOAT2 &v1, const float num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

static inline constexpr bool operator< (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y));
}

static inline constexpr bool operator> (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y));
}

static inline constexpr bool operator<= (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator>= (const DirectX::XMFLOAT2 &v1, const DirectX::XMFLOAT2 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr auto operator- (const DirectX::XMFLOAT2 &v1)
{
    return DirectX::XMFLOAT2{ -v1.x, -v1.y };
}

#pragma endregion

//-------------------------------------------------------------------------------------------------------------
// XMINT4ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMINT4

static inline void operator+= (DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;
}

static inline void operator-= (DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;
}

static inline void operator/= (DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
    v1.w /= v2.w;
}

static inline void operator*= (DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    v1.w *= v2.w;
}

static inline void operator%= (DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    v1.x = ::tmodt(v1.x, v2.x);
    v1.y = ::tmodt(v1.y, v2.y);
    v1.z = ::tmodt(v1.z, v2.z);
    v1.w = ::tmodt(v1.w, v2.w);
}

static inline constexpr auto operator+ (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return DirectX::XMINT4{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

static inline constexpr auto operator- (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return DirectX::XMINT4{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

static inline constexpr auto operator* (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return DirectX::XMINT4{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

static inline constexpr auto operator/ (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return DirectX::XMINT4{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}

static inline constexpr auto operator% (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return DirectX::XMINT4{ ::tmodt(v1.x, v2.x), ::tmodt(v1.y, v2.y), ::tmodt(v1.z, v2.z), ::tmodt(v1.w, v2.w) };
}

static inline constexpr auto operator+ (const DirectX::XMINT4 &v1, const int32_t num)
{
    return DirectX::XMINT4{ v1.x + num, v1.y + num, v1.z + num, v1.w + num };
}

static inline constexpr auto operator- (const DirectX::XMINT4 &v1, const int32_t num)
{
    return DirectX::XMINT4{ v1.x - num, v1.y - num, v1.z - num, v1.w - num };
}

static inline constexpr auto operator* (const DirectX::XMINT4 &v1, const int32_t num)
{
    return DirectX::XMINT4{ v1.x * num, v1.y * num, v1.z * num, v1.w * num };
}

static inline constexpr auto operator/ (const DirectX::XMINT4 &v1, const int32_t num)
{
    return DirectX::XMINT4{ v1.x / num, v1.y / num, v1.z / num, v1.w / num };
}

static inline constexpr auto operator% (const DirectX::XMINT4 &v1, const int32_t num)
{
    return DirectX::XMINT4{ ::tmodt(v1.x, num), ::tmodt(v1.y, num), ::tmodt(v1.z, num), ::tmodt(v1.w, num) };
}

static inline constexpr auto operator+ (const int32_t num, DirectX::XMINT4 &v1)
{
    return DirectX::XMINT4{ num + v1.x, num + v1.y, num + v1.z, num + v1.w };
}

static inline constexpr auto operator- (const int32_t num, DirectX::XMINT4 &v1)
{
    return DirectX::XMINT4{ num - v1.x, num - v1.y, num - v1.z, num - v1.w };
}

static inline constexpr auto operator* (const int32_t num, DirectX::XMINT4 &v1)
{
    return DirectX::XMINT4{ num * v1.x, num * v1.y, num * v1.z, num * v1.w };
}

static inline constexpr auto operator/ (const int32_t num, DirectX::XMINT4 &v1)
{
    return DirectX::XMINT4{ num / v1.x, num / v1.y, num / v1.z, num / v1.w };
}

static inline constexpr auto operator% (const int32_t num, DirectX::XMINT4 &v1)
{
    return DirectX::XMINT4{ tmodt(num, v1.x), tmodt(num, v1.y), tmodt(num, v1.z), tmodt(num, v1.w) };
}

static inline constexpr bool operator== (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator!= (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator== (const DirectX::XMINT4 &v1, const int32_t num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

static inline constexpr bool operator!= (const DirectX::XMINT4 &v1, const int32_t num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

static inline constexpr bool operator< (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w));
}

static inline constexpr bool operator> (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w));
}

static inline constexpr bool operator<= (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator>= (const DirectX::XMINT4 &v1, const DirectX::XMINT4 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr auto operator- (const DirectX::XMINT4 &v1)
{
    return DirectX::XMINT4{ -v1.x, -v1.y, -v1.z, -v1.w };
}

#pragma endregion
//-------------------------------------------------------------------------------------------------------------
// XMINT3ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMINT3

static inline void operator+= (DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
}

static inline void operator-= (DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
}

static inline void operator/= (DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
}

static inline void operator*= (DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
}

static inline void operator%= (DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    v1.x = ::tmodt(v1.x, v2.x);
    v1.y = ::tmodt(v1.y, v2.y);
    v1.z = ::tmodt(v1.z, v2.z);
}

static inline constexpr auto operator+ (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return DirectX::XMINT3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

static inline constexpr auto operator- (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return DirectX::XMINT3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

static inline constexpr auto operator* (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return DirectX::XMINT3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

static inline constexpr auto operator/ (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return DirectX::XMINT3{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

static inline constexpr auto operator% (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return DirectX::XMINT3{ ::tmodt(v1.x, v2.x), ::tmodt(v1.y, v2.y), ::tmodt(v1.z, v2.z) };
}

static inline constexpr auto operator+ (const DirectX::XMINT3 &v1, const int32_t num)
{
    return DirectX::XMINT3{ v1.x + num, v1.y + num, v1.z + num };
}

static inline constexpr auto operator- (const DirectX::XMINT3 &v1, const int32_t num)
{
    return DirectX::XMINT3{ v1.x - num, v1.y - num, v1.z - num };
}

static inline constexpr auto operator* (const DirectX::XMINT3 &v1, const int32_t num)
{
    return DirectX::XMINT3{ v1.x * num, v1.y * num, v1.z * num };
}

static inline constexpr auto operator/ (const DirectX::XMINT3 &v1, const int32_t num)
{
    return DirectX::XMINT3{ v1.x / num, v1.y / num, v1.z / num };
}

static inline constexpr auto operator% (const DirectX::XMINT3 &v1, const int32_t num)
{
    return DirectX::XMINT3{ ::tmodt(v1.x, num), ::tmodt(v1.y, num), ::tmodt(v1.z, num) };
}

static inline constexpr auto operator+ (const int32_t num, DirectX::XMINT3 &v1)
{
    return DirectX::XMINT3{ num + v1.x, num + v1.y, num + v1.z };
}

static inline constexpr auto operator- (const int32_t num, DirectX::XMINT3 &v1)
{
    return DirectX::XMINT3{ num - v1.x, num - v1.y, num - v1.z };
}

static inline constexpr auto operator* (const int32_t num, DirectX::XMINT3 &v1)
{
    return DirectX::XMINT3{ num * v1.x, num * v1.y, num * v1.z };
}

static inline constexpr auto operator/ (const int32_t num, DirectX::XMINT3 &v1)
{
    return DirectX::XMINT3{ num / v1.x, num / v1.y, num / v1.z };
}

static inline constexpr auto operator% (const int32_t num, DirectX::XMINT3 &v1)
{
    return DirectX::XMINT3{ tmodt(num, v1.x), tmodt(num, v1.y), tmodt(num, v1.z) };
}

static inline constexpr bool operator== (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator!= (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator== (const DirectX::XMINT3 &v1, const int32_t num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

static inline constexpr bool operator!= (const DirectX::XMINT3 &v1, const int32_t num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

static inline constexpr bool operator< (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z));
}

static inline constexpr bool operator> (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z));
}

static inline constexpr bool operator<= (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator>= (const DirectX::XMINT3 &v1, const DirectX::XMINT3 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr auto operator- (const DirectX::XMINT3 &v1)
{
    return DirectX::XMINT3{ -v1.x, -v1.y, -v1.z };
}

#pragma endregion
//-------------------------------------------------------------------------------------------------------------
// XMINT2ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMINT2

static inline void operator+= (DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
}

static inline void operator-= (DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
}

static inline void operator/= (DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
}

static inline void operator*= (DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
}

static inline void operator%= (DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    v1.x = ::tmodt(v1.x, v2.x);
    v1.y = ::tmodt(v1.y, v2.y);
}

static inline constexpr auto operator+ (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return DirectX::XMINT2{ v1.x + v2.x, v1.y + v2.y };
}

static inline constexpr auto operator- (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return DirectX::XMINT2{ v1.x - v2.x, v1.y - v2.y };
}

static inline constexpr auto operator* (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return DirectX::XMINT2{ v1.x * v2.x, v1.y * v2.y };
}

static inline constexpr auto operator/ (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return DirectX::XMINT2{ v1.x / v2.x, v1.y / v2.y };
}

static inline constexpr auto operator% (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return DirectX::XMINT2{ ::tmodt(v1.x, v2.x), ::tmodt(v1.y, v2.y) };
}

static inline constexpr auto operator+ (const DirectX::XMINT2 &v1, const int32_t num)
{
    return DirectX::XMINT2{ v1.x + num, v1.y + num };
}

static inline constexpr auto operator- (const DirectX::XMINT2 &v1, const int32_t num)
{
    return DirectX::XMINT2{ v1.x - num, v1.y - num };
}

static inline constexpr auto operator* (const DirectX::XMINT2 &v1, const int32_t num)
{
    return DirectX::XMINT2{ v1.x * num, v1.y * num };
}

static inline constexpr auto operator/ (const DirectX::XMINT2 &v1, const int32_t num)
{
    return DirectX::XMINT2{ v1.x / num, v1.y / num };
}

static inline constexpr auto operator% (const DirectX::XMINT2 &v1, const int32_t num)
{
    return DirectX::XMINT2{ ::tmodt(v1.x, num), ::tmodt(v1.y, num) };
}

static inline constexpr auto operator+ (const int32_t num, DirectX::XMINT2 &v1)
{
    return DirectX::XMINT2{ num + v1.x, num + v1.y };
}

static inline constexpr auto operator- (const int32_t num, DirectX::XMINT2 &v1)
{
    return DirectX::XMINT2{ num - v1.x, num - v1.y };
}

static inline constexpr auto operator* (const int32_t num, DirectX::XMINT2 &v1)
{
    return DirectX::XMINT2{ num * v1.x, num * v1.y };
}

static inline constexpr auto operator/ (const int32_t num, DirectX::XMINT2 &v1)
{
    return DirectX::XMINT2{ num / v1.x, num / v1.y };
}

static inline constexpr auto operator% (const int32_t num, DirectX::XMINT2 &v1)
{
    return DirectX::XMINT2{ tmodt(num, v1.x), tmodt(num, v1.y) };
}

static inline constexpr bool operator== (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator!= (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator== (const DirectX::XMINT2 &v1, const int32_t num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

static inline constexpr bool operator!= (const DirectX::XMINT2 &v1, const int32_t num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

static inline constexpr bool operator< (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y));
}

static inline constexpr bool operator> (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y));
}

static inline constexpr bool operator<= (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator>= (const DirectX::XMINT2 &v1, const DirectX::XMINT2 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr auto operator- (const DirectX::XMINT2 &v1)
{
    return DirectX::XMINT2{ -v1.x, -v1.y };
}

#pragma endregion

//-------------------------------------------------------------------------------------------------------------
// XMUINT4ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMUINT4

static inline void operator+= (DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;
}

static inline void operator-= (DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;
}

static inline void operator/= (DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
    v1.w /= v2.w;
}

static inline void operator*= (DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    v1.w *= v2.w;
}

static inline void operator%= (DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    v1.x = ::utmodut(v1.x, v2.x);
    v1.y = ::utmodut(v1.y, v2.y);
    v1.z = ::utmodut(v1.z, v2.z);
    v1.w = ::utmodut(v1.w, v2.w);
}

static inline constexpr auto operator+ (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return DirectX::XMUINT4{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

static inline constexpr auto operator- (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return DirectX::XMUINT4{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

static inline constexpr auto operator* (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return DirectX::XMUINT4{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

static inline constexpr auto operator/ (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return DirectX::XMUINT4{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}

static inline constexpr auto operator% (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return DirectX::XMUINT4{ ::utmodut(v1.x, v2.x), ::utmodut(v1.y, v2.y), ::utmodut(v1.z, v2.z), ::utmodut(v1.w, v2.w) };
}

static inline constexpr auto operator+ (const DirectX::XMUINT4 &v1, const uint32_t num)
{
    return DirectX::XMUINT4{ v1.x + num, v1.y + num, v1.z + num, v1.w + num };
}

static inline constexpr auto operator- (const DirectX::XMUINT4 &v1, const uint32_t num)
{
    return DirectX::XMUINT4{ v1.x - num, v1.y - num, v1.z - num, v1.w - num };
}

static inline constexpr auto operator* (const DirectX::XMUINT4 &v1, const uint32_t num)
{
    return DirectX::XMUINT4{ v1.x * num, v1.y * num, v1.z * num, v1.w * num };
}

static inline constexpr auto operator/ (const DirectX::XMUINT4 &v1, const uint32_t num)
{
    return DirectX::XMUINT4{ v1.x / num, v1.y / num, v1.z / num, v1.w / num };
}

static inline constexpr auto operator% (const DirectX::XMUINT4 &v1, const uint32_t num)
{
    return DirectX::XMUINT4{ ::utmodut(v1.x, num), ::utmodut(v1.y, num), ::utmodut(v1.z, num), ::utmodut(v1.w, num) };
}

static inline constexpr auto operator+ (const uint32_t num, DirectX::XMUINT4 &v1)
{
    return DirectX::XMUINT4{ num + v1.x, num + v1.y, num + v1.z, num + v1.w };
}

static inline constexpr auto operator- (const uint32_t num, DirectX::XMUINT4 &v1)
{
    return DirectX::XMUINT4{ num - v1.x, num - v1.y, num - v1.z, num - v1.w };
}

static inline constexpr auto operator* (const uint32_t num, DirectX::XMUINT4 &v1)
{
    return DirectX::XMUINT4{ num * v1.x, num * v1.y, num * v1.z, num * v1.w };
}

static inline constexpr auto operator/ (const uint32_t num, DirectX::XMUINT4 &v1)
{
    return DirectX::XMUINT4{ num / v1.x, num / v1.y, num / v1.z, num / v1.w };
}

static inline constexpr auto operator% (const uint32_t num, DirectX::XMUINT4 &v1)
{
    return DirectX::XMUINT4{ utmodut(num, v1.x), utmodut(num, v1.y), utmodut(num, v1.z), utmodut(num, v1.w) };
}

static inline constexpr bool operator== (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator!= (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator== (const DirectX::XMUINT4 &v1, const uint32_t num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

static inline constexpr bool operator!= (const DirectX::XMUINT4 &v1, const uint32_t num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

static inline constexpr bool operator< (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w));
}

static inline constexpr bool operator> (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w));
}

static inline constexpr bool operator<= (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static inline constexpr bool operator>= (const DirectX::XMUINT4 &v1, const DirectX::XMUINT4 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

#pragma endregion
//-------------------------------------------------------------------------------------------------------------
// XMUINT3ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMUINT3

static inline void operator+= (DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
}

static inline void operator-= (DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
}

static inline void operator/= (DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
}

static inline void operator*= (DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
}

static inline void operator%= (DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    v1.x = ::utmodut(v1.x, v2.x);
    v1.y = ::utmodut(v1.y, v2.y);
    v1.z = ::utmodut(v1.z, v2.z);
}

static inline constexpr auto operator+ (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return DirectX::XMUINT3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

static inline constexpr auto operator- (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return DirectX::XMUINT3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

static inline constexpr auto operator* (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return DirectX::XMUINT3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

static inline constexpr auto operator/ (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return DirectX::XMUINT3{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

static inline constexpr auto operator% (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return DirectX::XMUINT3{ ::utmodut(v1.x, v2.x), ::utmodut(v1.y, v2.y), ::utmodut(v1.z, v2.z) };
}

static inline constexpr auto operator+ (const DirectX::XMUINT3 &v1, const uint32_t num)
{
    return DirectX::XMUINT3{ v1.x + num, v1.y + num, v1.z + num };
}

static inline constexpr auto operator- (const DirectX::XMUINT3 &v1, const uint32_t num)
{
    return DirectX::XMUINT3{ v1.x - num, v1.y - num, v1.z - num };
}

static inline constexpr auto operator* (const DirectX::XMUINT3 &v1, const uint32_t num)
{
    return DirectX::XMUINT3{ v1.x * num, v1.y * num, v1.z * num };
}

static inline constexpr auto operator/ (const DirectX::XMUINT3 &v1, const uint32_t num)
{
    return DirectX::XMUINT3{ v1.x / num, v1.y / num, v1.z / num };
}

static inline constexpr auto operator% (const DirectX::XMUINT3 &v1, const uint32_t num)
{
    return DirectX::XMUINT3{ ::utmodut(v1.x, num), ::utmodut(v1.y, num), ::utmodut(v1.z, num) };
}

static inline constexpr auto operator+ (const uint32_t num, DirectX::XMUINT3 &v1)
{
    return DirectX::XMUINT3{ num + v1.x, num + v1.y, num + v1.z };
}

static inline constexpr auto operator- (const uint32_t num, DirectX::XMUINT3 &v1)
{
    return DirectX::XMUINT3{ num - v1.x, num - v1.y, num - v1.z };
}

static inline constexpr auto operator* (const uint32_t num, DirectX::XMUINT3 &v1)
{
    return DirectX::XMUINT3{ num * v1.x, num * v1.y, num * v1.z };
}

static inline constexpr auto operator/ (const uint32_t num, DirectX::XMUINT3 &v1)
{
    return DirectX::XMUINT3{ num / v1.x, num / v1.y, num / v1.z };
}

static inline constexpr auto operator% (const uint32_t num, DirectX::XMUINT3 &v1)
{
    return DirectX::XMUINT3{ utmodut(num, v1.x), utmodut(num, v1.y), utmodut(num, v1.z) };
}

static inline constexpr bool operator== (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator!= (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator== (const DirectX::XMUINT3 &v1, const uint32_t num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

static inline constexpr bool operator!= (const DirectX::XMUINT3 &v1, const uint32_t num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

static inline constexpr bool operator< (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z));
}

static inline constexpr bool operator> (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z));
}

static inline constexpr bool operator<= (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static inline constexpr bool operator>= (const DirectX::XMUINT3 &v1, const DirectX::XMUINT3 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

#pragma endregion
//-------------------------------------------------------------------------------------------------------------
// XMUINT2ån
//-------------------------------------------------------------------------------------------------------------
#pragma region XMUINT2

static inline void operator+= (DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
}

static inline void operator-= (DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
}

static inline void operator/= (DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
}

static inline void operator*= (DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
}

static inline void operator%= (DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    v1.x = ::utmodut(v1.x, v2.x);
    v1.y = ::utmodut(v1.y, v2.y);
}

static inline constexpr auto operator+ (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return DirectX::XMUINT2{ v1.x + v2.x, v1.y + v2.y };
}

static inline constexpr auto operator- (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return DirectX::XMUINT2{ v1.x - v2.x, v1.y - v2.y };
}

static inline constexpr auto operator* (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return DirectX::XMUINT2{ v1.x * v2.x, v1.y * v2.y };
}

static inline constexpr auto operator/ (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return DirectX::XMUINT2{ v1.x / v2.x, v1.y / v2.y };
}

static inline constexpr auto operator% (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return DirectX::XMUINT2{ ::utmodut(v1.x, v2.x), ::utmodut(v1.y, v2.y) };
}

static inline constexpr auto operator+ (const DirectX::XMUINT2 &v1, const uint32_t num)
{
    return DirectX::XMUINT2{ v1.x + num, v1.y + num };
}

static inline constexpr auto operator- (const DirectX::XMUINT2 &v1, const uint32_t num)
{
    return DirectX::XMUINT2{ v1.x - num, v1.y - num };
}

static inline constexpr auto operator* (const DirectX::XMUINT2 &v1, const uint32_t num)
{
    return DirectX::XMUINT2{ v1.x * num, v1.y * num };
}

static inline constexpr auto operator/ (const DirectX::XMUINT2 &v1, const uint32_t num)
{
    return DirectX::XMUINT2{ v1.x / num, v1.y / num };
}

static inline constexpr auto operator% (const DirectX::XMUINT2 &v1, const uint32_t num)
{
    return DirectX::XMUINT2{ ::utmodut(v1.x, num), ::utmodut(v1.y, num) };
}

static inline constexpr auto operator+ (const uint32_t num, DirectX::XMUINT2 &v1)
{
    return DirectX::XMUINT2{ num + v1.x, num + v1.y };
}

static inline constexpr auto operator- (const uint32_t num, DirectX::XMUINT2 &v1)
{
    return DirectX::XMUINT2{ num - v1.x, num - v1.y };
}

static inline constexpr auto operator* (const uint32_t num, DirectX::XMUINT2 &v1)
{
    return DirectX::XMUINT2{ num * v1.x, num * v1.y };
}

static inline constexpr auto operator/ (const uint32_t num, DirectX::XMUINT2 &v1)
{
    return DirectX::XMUINT2{ num / v1.x, num / v1.y };
}

static inline constexpr auto operator% (const uint32_t num, DirectX::XMUINT2 &v1)
{
    return DirectX::XMUINT2{ utmodut(num, v1.x), utmodut(num, v1.y) };
}

static inline constexpr bool operator== (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator!= (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator== (const DirectX::XMUINT2 &v1, const uint32_t num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

static inline constexpr bool operator!= (const DirectX::XMUINT2 &v1, const uint32_t num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

static inline constexpr bool operator< (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y));
}

static inline constexpr bool operator> (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y));
}

static inline constexpr bool operator<= (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static inline constexpr bool operator>= (const DirectX::XMUINT2 &v1, const DirectX::XMUINT2 &v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}