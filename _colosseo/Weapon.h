#pragma once
#include "State.h"
#include "Collision.h"

enum Weapons{
	W_None, //�f��
	W_NormalGun //�f�t�H�e
};

enum WeaponTypes {
	Melee, //�ߐ�
	Ranged //������
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
	int NowWeapon; //���ݓK������Ă��镐��

	int TickTime;

	bool AttackFlag = false;

	void Update(bool Trigger);
	void Attack();

	void SetWeapon(int WeaponNum);
};

