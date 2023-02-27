#pragma once
#include "Cards.h"
#include "CardGetEff.h"

class Player;

class Shop {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	// Card�Ǘ��N���X�̃|�C���^
	Cards* m_pCards;
	Player* m_pPlayer;
	// �w�i�e�N�X�`��
	int BackTex;

	// ����
	bool m_CanSelect;
	// ��
	int m_Heal_Tex;
	XMFLOAT2 m_Heal_LT;
	XMFLOAT2 m_Heal_Size;
	// �񕜉\���ǂ���
	bool m_CanHeal;
	void Heal_Update();
	// �J�[�h�l��
	int m_Card_Get_Tex;
	XMFLOAT2 m_Card_Get_LT;
	XMFLOAT2 m_Card_Get_Size;
	// �J�[�h�l���\���ǂ���
	bool m_CanCard_Get;
	// ���̍ő吔
	static const int CHOICES_COUNT = 3;
	// ���J�[�h
	CardType m_Choices_Card[CHOICES_COUNT];
	// �J�[�h�l����I��������
	bool m_IsSelected_Card_Flag;
	// �J�[�h�擾�G�t�F�N�g
	std::unique_ptr<CardGetEff> m_CardEffPtr;
	void Card_Get_Update();

	// �I���I��������
	bool m_IsSelectPheseEnd;
	// �I����
	bool m_IsEnd;
public:
	Shop();
	~Shop();

	bool GetEnd() { return m_IsEnd; }

	void SetCardsPtr(Cards* _pCards);
	void SetPlayerPtr(Player* _pPlayer);

	void Update();
	void Draw();
};