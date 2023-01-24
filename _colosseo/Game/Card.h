#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "Collider/GameCollisionManager.h"

class Player;
class Enemys;
class Map;
class Cards;

//-------------------------------------------------------------------------------------------------------------
// �J�[�h���
//-------------------------------------------------------------------------------------------------------------
enum class CardType {
	NONE,
	DEFAULT_ATTACK,
	DEFAULT_MOVE,
	NEXT_EN_JUMP
};

//-------------------------------------------------------------------------------------------------------------
// �J�[�h
//-------------------------------------------------------------------------------------------------------------
class Card
{
protected:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected:

	Player* pPlayer = nullptr;
	Enemys* pEnemys = nullptr;
	Map* pMap = nullptr;

	CardType m_Type = CardType::NONE;
	float width = 0;
	float height = 0;
	XMFLOAT2 LeftTop = XMFLOAT2();

	int Graph = 0;

public:
		GameBaseCollider *collider = nullptr;
		virtual ~Card();
		void SetCollider();
		void ColliderUpdate();
		//GameBoxCollider *col = nullptr;
public:
	bool HitCursor = false;
	bool Active = false;
	Card();
	//Card(CardType Type);

	CardType GetType() { return m_Type; }

	void Initialize(int index, int MaxIdx);
	void Update(int index, int MaxIdx);

	
	virtual void Draw(int index, int MaxIdx);

	// ����������
	void ConstructInitialize();
	// �|�C���^�Z�b�g
	void SetPlEnPtr(Player* _pPlayer, Enemys* _pEnemys, Map* _pMap);

	// -----------------------------------
	// �J�[�h�p�����[�^
	// -----------------------------------
public:
	// ���ݑI�𒆂̃}�X
	XMINT2 m_NowCheckChip = XMINT2();
	// �U�������ꍇ�̃_���[�W
	int m_PreDamage = 0;
	// �t�F�[�Y��
	int m_PheseCount = 3;


	// -----------------------------------
	// �t�F�[�Y�Ǘ��p
	// -----------------------------------
public:
	bool m_PheseChange = false;
protected:
	// �ҋ@�^�C�}�[
	int m_WaitTimer;
	// �t�F�[�Y�J�n���̃g���K�[�t���O
	bool m_PheseStartTriggerFlag;
	// �t�F�[�Y�I���t���O
	bool m_isPheseEnd;
	// �ҋ@�^�C�}�[��0�ł��t�F�[�Y�I���t���O���オ���Ă���ꍇ�� true ��Ԃ�
	// ���łɑҋ@�^�C�}�[�̍X�V���s��
	bool CanPhaseEnd();

	/// <summary>
	/// ���݂̃t�F�[�Y�� _WaitFlame �t���[����ɏI��������
	/// </summary>
	/// <param name="_WaitFlame">�I���܂ł̃t���[����</param>
	void PheseEndFunc(int _WaitFlame);

	// -----------------------------------
	// �t�F�[�Y�Ǘ��p�ϐ��擾
	// -----------------------------------
public:


	// -----------------------------------
	// �t�F�[�Y�������֐���
	// -----------------------------------
public:


	// -----------------------------------
	// �����t�F�[�Y1
	// -----------------------------------
	
	// �t�F�[�Y1�J�n���g���K�[����
	virtual void Phese1Start() = 0;
	// �t�F�[�Y1�X�V����
	virtual void UpdatePhese1() = 0;
	// �t�F�[�Y1�I������
	virtual void Phese1End() = 0;


	// -----------------------------------
	// �����t�F�[�Y2
	// -----------------------------------
	
	// �t�F�[�Y2�J�n���g���K�[����
	virtual void Phese2Start() = 0;
	// �t�F�[�Y2�X�V����
	virtual void UpdatePhese2() = 0;
	// �t�F�[�Y2�I������
	virtual void Phese2End() = 0;


	// -----------------------------------
	// �����t�F�[�Y3
	// -----------------------------------
	
	// �t�F�[�Y3�J�n���g���K�[����
	virtual void Phese3Start() = 0;
	// �t�F�[�Y3�X�V����
	virtual void UpdatePhese3() = 0;
	// �t�F�[�Y3�I������
	virtual void Phese3End() = 0;

	
	/// <summary>
	/// �J�[�h�I�����g���K�[����
	/// </summary>
	/// /// <param name="RefPoint">�I���\�}�X�𐶐������̃}�b�v���W</param>
	/// <param name="AddCanSelectChips">Chips�N���X</param>
	virtual void SelectTrigger(XMINT2 RefPoint, Cards* pCards) = 0;

	// �I���}�X�̃Z�b�g
	virtual void SetSelectData(XMINT2 _Now, XMINT2 _Pre) = 0;
};

