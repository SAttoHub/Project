#pragma once
#include <vector>
#include "Card.h"
#include "..\Engine\Common\Containers\MyFList.h"
#include "Player.h"
#include "Enemys.h"
#include "Map.h"

//-------------------------------------------------------------------------------------------------------------
// �J�[�h�Ǘ�(�f�b�L)
//-------------------------------------------------------------------------------------------------------------
class Cards
{
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//My_F_List<Card*> m_Cards;
	//std::vector<Card*> m_Cards;
	std::vector<std::unique_ptr<Card>> m_Cards;

private:
	Player *pPlayer = nullptr;
	Enemys *pEnemys = nullptr;
	Map *pMap = nullptr;

	int TurnEndButton = 0;
	bool isMyTurn = false;

	// �U���ŗ^����_���[�W�̗\�z�l(�h��͌v�Z�O)
	int PreDamage = 0;

	// �I����x�ڂ̓J�[�h�𔭓����Ȃ��ׂ̕ϐ�
	XMINT2 PreCheckChipOld = XMINT2(-1, -1);
	XMINT2 PreCheckChip = XMINT2(-2, -2);
	bool PreChipExistEnemy = false;

	// ���̃^�C�}�[��0�ł͂Ȃ��Ԃ͓��͂��󂯕t���Ȃ�
	int WaitTimer = 0;
	void Wait(int Value) { WaitTimer = Value; }
public:
	//std::vector<Card> m_Cards;
	
	std::vector<int> m_Deck;

	// 0 : �J�[�h�I��  1 : �J�[�h�����@2 : �J�[�h�㏈��(�ړ���̌����ύX��)
	int NowPhase = 0;
	CardType m_UseCardType = CardType::NONE;
	int m_UseCardIndex = 0;
	XMINT2 m_NowSelectChip = XMINT2();
	std::vector<XMINT2> CanSelectChips;

	Cards();
	~Cards();

	// �^�[���G���h�{�^��
	XMFLOAT2 LT = XMFLOAT2();
	//~Cards();

	void Initialize(Player *_Player, Enemys *_Enemys, Map *_Map);
	void Update();
	void Draw();

	bool TurnEnd = false;
	void StartTurn();

	bool ExistInCanSelectChips(XMINT2 MapPos);
	void AddCanSelectChips(XMINT2 MapPos);
	void AddCanSelectChips_Move(XMINT2 MapPos);

	void PlayerTurnUpdate();
	void CardPhaseUpdate();
	void CardEffectPhaseUpdate();
	void CardAfterPhaseUpdate();

	void Reset(); // �����Z�b�g

public:
	// �U���ŗ^����_���[�W�̗\�z�l(�h��͌v�Z�O)�Q�b�^
	int GetPreDamage() { return PreDamage; }
};

