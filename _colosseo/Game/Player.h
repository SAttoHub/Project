#pragma once
#include "Charactor.h"
#include "Map.h"

//-------------------------------------------------------------------------------------------------------------
// �v���C���[�L�����N�^�[
//-------------------------------------------------------------------------------------------------------------
class Player : public Charactor
{

public:
	Player();
	~Player();

	void SetMap(Map *Map);


	void Update() override;
	void Draw() override;

	void Reset(); // �����Z�b�g
};

