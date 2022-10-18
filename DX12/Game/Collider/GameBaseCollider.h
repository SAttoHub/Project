#pragma once
#include <string>
#include "..\..\Engine\CommonMathIncludes.h"
#include "..\..\Engine\Primitive\PrimitiveFunc.h"
#include "..\..\Engine\Common\Colors\ColorFuncs.h"

enum GameCollisionShapeType
{
	COL_SHAPE_UNKNOWN = -1,		// ���ݒ�
	COL_SHAPE_CIRCLE,			// �~
	COL_SHAPE_BOX,				// ��
	COL_SHAPE_POINT,			// �_

	COL_3D_SHAPE_RAY,
	COL_3D_SHAPE_MOUSERAY,
	COL_3D_SHAPE_CUBE,
	COL_3D_SHAPE_PLANE
};

// �Փ˔���g���p Class�@������p�������I�u�W�F�N�g�ɑ΂��ďՓ˂������ɃR���C�_�[���擾�ł���
class ColliderProp {
public:
	virtual ~ColliderProp() {}
};

class GameBaseCollider {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	GameBaseCollider() = default;
	virtual ~GameBaseCollider() = default;

	inline void SetBasisPosition(XMFLOAT2 *Position) {
		this->Position = Position;
	}

	inline XMFLOAT2 *GetBasisPosition() {
		return Position;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	inline GameCollisionShapeType GetShapeType() { return shapeType; }
	inline std::string GetTag() { return Tag; }

	/// <summary>
	/// �Փ˔���֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const GameCollisionShapeType &info);

protected:
	// ��_���W
	XMFLOAT2 *Position = nullptr;
	// �`��^�C�v
	GameCollisionShapeType shapeType = COL_SHAPE_UNKNOWN;

public:
	//�^�O
	std::string Tag = "None";

	// �Փ˔���g��
	// �R���C�_�[�����I�u�W�F�N�g�̐e�N���X�̃|�C���^
	ColliderProp *Base;
	// �R���C�_�[�����I�u�W�F�N�g�̐e�N���X�̃|�C���^���Z�b�g����
	void SetBaseProp(ColliderProp *Base) {
		this->Base = Base;
	}
};

