#pragma once
#include "../Engine/CommonMathIncludes.h"

class Enemy;
class Player;

class ActionEffectBase {
protected:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

	// �J�������_�p�̃^�[�Q�b�g���W
	XMFLOAT3 m_Target;

	int m_Timer = 0;
	int m_EndTime = 0;

	// �^�C�}�[��i�߂�
	void TimerUpdate();
public:

	virtual void Start(int _Timer, int _EndTime) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// �I���t���O
	bool m_isEnd = false;
};