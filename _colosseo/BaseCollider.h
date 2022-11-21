#pragma once
#include "Engine\Objects\3DObject.h"
#include "CollisionInfo.h"

enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // ���ݒ�
	COLLISIONSHAPE_SPHERE, // ��
	COLLISIONSHAPE_MESH, // ���b�V��
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
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo &info) {
		object3d->OnCollision(info);
	}

protected:
	Object3d *object3d = nullptr;
	// �`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};
