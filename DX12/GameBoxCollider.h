#pragma once
#include "GameBaseCollider.h"
#include "CollisionPrimitive.h"

class GameBoxCollider : public GameBaseCollider, public Box
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	GameBoxCollider(XMFLOAT2 *BasisPosition, XMFLOAT2 pos1 = { 0,0 }, XMFLOAT2 pos2 = { 0,0 }, std::string tag = "None") :
		Pos1(pos1),
		Pos2(pos2)
	{
		// ��_�|�C���^�̍X�V
		Position = BasisPosition;
		// ���̃����o�ϐ����X�V
		Box::pos1 = Pos1 + *Position;
		Box::pos2 = Pos2 + *Position;
		// �^�O�̐ݒ�
		GameBaseCollider::Tag = tag;
		// ���`����Z�b�g
		shapeType = COL_SHAPE_BOX;
	}

	~GameBoxCollider(){
		if (Base != nullptr) {
			delete Base;
		}
		if(Position != nullptr){
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

	/// <summary>
	/// ��_�� 0,0 �Ƃ��Ĕ��̃T�C�Y��ύX����
	/// </summary>
	/// <param name="pos1">������W</param>
	/// <param name="pos2">�E�����W</param>
	inline void SetBoxSize(XMFLOAT2 pos1, XMFLOAT2 pos2);

private:
	// ������W( ���S���W�����0�Ƃ�������) �ړ����͎����Ōv�Z
	XMFLOAT2 Pos1;
	// �E�����W( ���S���W�����0�Ƃ�������) �ړ����͎����Ōv�Z
	XMFLOAT2 Pos2;
};

