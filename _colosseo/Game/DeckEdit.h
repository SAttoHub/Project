#pragma once
#include "Cards.h"

//-------------------------------------------------------------------------------------------------------------
// �f�b�L�ҏW�N���X
//-------------------------------------------------------------------------------------------------------------
class DeckEdit {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	// Card�Ǘ��N���X�̃|�C���^
	Cards* m_pCards;

	// �f�b�L�ҏW���j���[�t���O
	bool m_IsEditDeck;

	// �J�[�h�e�N�X�`��
	int CardTex[int(CardType::CARD_TYPE_MAX)];
	// �w�i�e�N�X�`��
	int BackTex;

	// �z�[���h���̃J�[�h���
	CardType m_HoldCardType;
	// �z�[���h���̃J�[�h�̃C���f�b�N�X
	int m_HoldCardIndex;
	// �z�[���h���Ă���J�[�h���ǂ��̃J�[�h��
	int m_HoldCardOwner; // 0 �z�[���h�����@�@1 �f�b�L�@�@2�@�@�J�[�hBOX

public:
	DeckEdit();
	~DeckEdit();

	void SetCardsPtr(Cards* _pCards);

	void Udpate();
	void Draw();

private:
	// �f�b�L��Index�Ԗڂ̃J�[�h�^�C�v�̎擾
	CardType GetDeckCardIndcex(int _Index);
	// �J�[�hBOX��Index�Ԗڂ̃J�[�h�^�C�v�̎擾
	CardType GetBoxCardIndcex(int _Index);

	// �f�b�L�̃J�[�h�Ƃ̔���
	int CheckHitCusorDeck();
	// �J�[�hBOX�̃J�[�h�Ƃ̔���
	int CheckHitCusorBOX();

	// �f�b�L�J�[�h���g�`��
	void DrawDeckBack();
	// �f�b�L�J�[�h�`��
	void DrawDeckCard();

	// �J�[�hBOX���g�`��
	void DrawCardBoxBack();
	// �J�[�hBOX�J�[�h�`��
	void DrawCardBoxCard();
};