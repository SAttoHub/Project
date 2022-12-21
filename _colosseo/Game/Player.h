#pragma once
#include "Charactor.h"
#include "Map.h"

//-------------------------------------------------------------------------------------------------------------
// プレイヤーキャラクター
//-------------------------------------------------------------------------------------------------------------
class Player : public Charactor
{

public:
	Player();
	~Player();

	void SetMap(Map *Map);


	void Update() override;
	void Draw() override;

	void Reset(); // 仮リセット
};

