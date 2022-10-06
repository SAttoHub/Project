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
		// ���C�̃����o�ϐ����X�V
		Ray::start = ray_.start;
		Ray::dir = ray_.dir;
		// �^�O�̐ݒ�
		GameBaseCollider::Tag = tag;
		// ���`����Z�b�g
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
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

private:
	// �Ray
	Ray ray_;
};

