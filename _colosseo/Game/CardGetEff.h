#pragma once
#include "Cards.h"

class CardGetEff {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

	// ���肵���J�[�h
	CardType m_CardType;
	// �o�ߎ���
	int m_Timer;
	// �ő厞��
	static const int MAX_TIME = 30;
	// �I�����Ă����猚�Ă�
	bool m_isEnd;

	// �J�[�h���S���W
	XMFLOAT2 m_CenterPos;
	// �J�[�h�T�C�Y
	XMFLOAT2 m_CardSize;
	// �g�k�T�C�Y
	float m_Size;
public:
	CardGetEff(CardType _GetCardType);
	~CardGetEff();

	void Update();
	void Draw();

	// �I��������
	bool GetEnd() { return m_isEnd; }
};