#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Singleton.h"

//-------------------------------------------------------------------------------------------------------------
// �G�G�l�~�[�p�^�[�Q�b�g�|�C���g
//-------------------------------------------------------------------------------------------------------------
class TargetPoint : public Singleton<TargetPoint> {
	friend class Singleton<TargetPoint>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	XMFLOAT3 position = XMFLOAT3(); // �^�[�Q�b�g�|�C���g�̈ʒu

public:
	// �^�[�Q�b�g�|�C���g�̍��W���擾
	XMFLOAT3 GetPos() { return position; }
	// �^�[�Q�b�g�|�C���g�̍��W���Z�b�g
	void SetPos(XMFLOAT3 pos) { position = pos; }
};

// �^�[�Q�b�g�|�C���g�̍��W���擾
static DirectX::XMFLOAT3 GetTargetPos() {
	return TargetPoint::Instance()->GetPos();
}

// �^�[�Q�b�g�|�C���g�̍��W���Z�b�g
static void SetTargetPos(DirectX::XMFLOAT3 pos) {
	TargetPoint::Instance()->SetPos(pos);
}