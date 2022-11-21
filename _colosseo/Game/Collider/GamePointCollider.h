#pragma once
#include "GameBaseCollider.h"
#include "..\..\CollisionPrimitive.h"

//-------------------------------------------------------------------------------------------------------------
// �_�R���C�_�[
//-------------------------------------------------------------------------------------------------------------
class GamePointCollider : public GameBaseCollider, public Point
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	GamePointCollider(XMFLOAT2 *BasisPosition, std::string tag = "None")
	{
		// ��_�|�C���^�̍X�V
		Position = BasisPosition;
		// ���̃����o�ϐ����X�V
		Point::pos = *Position;
		// �^�O�̐ݒ�
		GameBaseCollider::Tag = tag;
		// ���`����Z�b�g
		shapeType = COL_SHAPE_POINT;
	}

	~GamePointCollider() {
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
};

