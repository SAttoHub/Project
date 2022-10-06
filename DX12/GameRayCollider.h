#pragma once
#include "GameBaseCollider.h"
#include "CollisionPrimitive.h"

class GameRayCollider : public GameBaseCollider, public Ray
{
	private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	GameRayCollider(Ray *BasisRay, std::string tag = "None")
	{
		ray_ = *BasisRay;
		// レイのメンバ変数を更新
		Ray::start = ray_.start;
		Ray::dir = ray_.dir;
		// タグの設定
		GameBaseCollider::Tag = tag;
		// 箱形状をセット
		shapeType = COL_3D_SHAPE_RAY;
	}

	~GameRayCollider() {
		if (Base != nullptr) {
			delete Base;
		}
		if (Position != nullptr) {
			delete Position;
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	// 基準Ray
	Ray ray_;
};

