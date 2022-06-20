#pragma once
#include "State.h"
#include "Collision.h"

enum Weapons{
	W_None, //素手
	W_NormalGun //デフォ銃
};

enum WeaponTypes {
	Melee, //近接
	Ranged //遠距離
};

class Weapon {
public:
	Weapon();
	~Weapon() {}

	int Ammo;
	int MAX_AMMO;
	int ATK;
	int FireRate;
	int WeaponType;
	int NowWeapon; //現在適応されている武器

	int TickTime;

	bool AttackFlag = false;

	void Update(bool Trigger);
	void Attack();

	void SetWeapon(int WeaponNum);
};

