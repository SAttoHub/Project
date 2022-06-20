#pragma once
#include "GameBaseCollider.h"
#include "CollisionPrimitive.h"

class GameCircleCollider : public GameBaseCollider, public Circle
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	GameCircleCollider(XMFLOAT2 *BasisPosition, XMFLOAT2 center = { 0,0 }, float radius = 1.0f, std::string tag = "None") :
		Center(center),
		Radius(radius)
	{
		// ��_�|�C���^�̍X�V
		Position = BasisPosition;
		Center = *Position;
		// �~�̃����o�ϐ����X�V
		Circle::center = Center;
		Circle::radius = Radius;
		// �^�O�̐ݒ�
		GameBaseCollider::Tag = tag;
		// �~�`����Z�b�g
		shapeType = COL_SHAPE_CIRCLE;
	}

	~GameCircleCollider() {
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
	/// �`��@�~��Draw�͍���ĂȂ��̂ŋ@�\���܂���
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �~�̔��a��ύX����
	/// </summary>
	/// <param name="radius">���a</param>
	inline void SetRadius(float radius) { this->Radius = radius; }

private:
	XMFLOAT2 Center;
	float Radius;
};

