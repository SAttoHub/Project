#pragma once
#include "CollisionPrimitive.h"

class Collision2 {
public:
	//2D
	static bool CheckCircleCircle(const Circle &circleA, const Circle &circleB);
	static bool CheckCircleBox(const Circle &circle, const Box &box);
	static bool CheckBoxBox(const Box &boxA, const Box &boxB);
	static bool CheckBoxPoint(const Box &box, const Point &point);
	static bool CheckCirclePoint(const Circle &circle, const Point &point);
	//3D
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest);
	static bool CheckSphere2Sphere(const Sphere &sphereA, const Sphere &sphereB, DirectX::XMVECTOR *inter = nullptr);
	static bool CheckSphere2Plane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter = nullptr);
	static bool CheckSphere2Triangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter = nullptr);
	static bool CheckRay2Plane(const Ray &lay, const Plane &plane, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);
	static bool CheckRay2Triangle(const Ray &lay, const Triangle &triangle, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);
	static bool CheckRay2Sphere(const Ray &lay, const Sphere &sphere, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);
};