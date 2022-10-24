#pragma once
#include "Charactor.h"
#include "Map.h"
//#include "Enemy.h"
class Player : public Charactor
{
private:
	Map *pMap = nullptr;

public:
	Player();
	~Player();

	void SetMap(Map *Map);

	void Update() override;
	void Draw() override;

	void Reset(); // ‰¼ƒŠƒZƒbƒg
};

