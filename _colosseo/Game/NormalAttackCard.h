#pragma once
#include "Card.h"

class Enemy;

class NormalAttackCard : public Card {

	int Timer = 0;
	Enemy* Pen;
public:
	// �R���X�g���N�^
	NormalAttackCard();
	// �f�X�g���N�^
	~NormalAttackCard();

	// �`��
	void Draw(int index, int MaxIdx) override;
	
	// -----------------------------------
	// �����t�F�[�Y1
	// -----------------------------------

	// �t�F�[�Y1�J�n���g���K�[����
	void Phese1Start() override;
	// �t�F�[�Y1�X�V����
	void UpdatePhese1() override;
	// �t�F�[�Y1�I������
	void Phese1End() override;


	// -----------------------------------
	// �����t�F�[�Y2
	// -----------------------------------

	// �t�F�[�Y2�J�n���g���K�[����
	void Phese2Start() override;
	// �t�F�[�Y2�X�V����
	void UpdatePhese2() override;
	// �t�F�[�Y2�I������
	void Phese2End() override;


	// -----------------------------------
	// �����t�F�[�Y3
	// -----------------------------------

	// �t�F�[�Y3�J�n���g���K�[����
	void Phese3Start() override;
	// �t�F�[�Y3�X�V����
	void UpdatePhese3() override;
	// �t�F�[�Y3�I������
	void Phese3End() override;

	/// <summary>
	/// �J�[�h�I�����g���K�[����
	/// </summary>
	/// /// <param name="RefPoint">�I���\�}�X�𐶐������̃}�b�v���W</param>
	/// <param name="AddCanSelectChips">Chips�N���X</param>
	void SelectTrigger(XMINT2 RefPoint, Cards* pCards) override;


	// -----------------------------------
	// �v���C���[�����ύX
	// -----------------------------------
	void ChangePlayerDir(Player* _pPlayer);


	// -----------------------------------
	// �����p�p�����[�^��
	// -----------------------------------

	// �O��I�����Ă����}�X
	XMINT2 m_PreCheckChip = XMINT2();
	// �I���}�X�̃Z�b�g
	void SetSelectData(XMINT2 _Now, XMINT2 _Pre) override;
};
