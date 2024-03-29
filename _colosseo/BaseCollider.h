#pragma once
#include "Engine\Objects\3DObject.h"
#include "CollisionInfo.h"

enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // 未設定
	COLLISIONSHAPE_SPHERE, // 球
	COLLISIONSHAPE_MESH, // メッシュ
};

class BaseCollider {
public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d *object) {
		this->object3d = object;
	}

	inline Object3d *GetObject3d() {
		return object3d;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo &info) {
		object3d->OnCollision(info);
	}

protected:
	Object3d *object3d = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};
