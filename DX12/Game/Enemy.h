#pragma once
#include "Charactor.h"
#include "Map.h"
#include "Astar.h"
#include "..\Input.h"

class Enemys;
class Player;

enum ActPattern {
	AC_Wait,
	AC_Move,
	AC_Attack
};

//-------------------------------------------------------------------------------------------------------------
// 敵
//-------------------------------------------------------------------------------------------------------------
class Enemy : public Charactor
{
	Player *pPlayer = nullptr;
	AstarResults m_Route = AstarResults(); // ルート保存用

	int m_CoolTime = 0;
	int m_Count = 0;
	ActPattern m_Act = ActPattern::AC_Wait;

public:
	// 攻撃のデータ構造体
	struct Enemy_Attack_Data {
		std::string m_Name;		// 技名
		int m_Damage;			// ダメージ
		int m_Cost;				// 使用コスト
		int m_KnockBackPower;	// ノックバック量
		Abnormality m_Abn;		// 与える状態異常
		int m_AbnTurn;			// 状態異常のターン

		/// <param name="Name"> 技名 </param>
		/// <param name="Damage"> ダメージ </param>
		/// <param name="Cost"> 使用コスト </param>
		/// <param name="KnockBackPower"> ノックバック量 </param>
		/// <param name="Abn"> 与える状態異常 </param>
		/// <param name="AbnTurn"> 状態異常のターン </param>
		Enemy_Attack_Data(std::string Name, int Damage, int Cost, int KnockBackPower, Abnormality Abn, int AbnTurn) :
			m_Name(Name), m_Damage(Damage), m_Cost(Cost), m_KnockBackPower(KnockBackPower), m_Abn(Abn), m_AbnTurn(AbnTurn) {};
	};
	std::vector<Enemy_Attack_Data> m_Attack_Datas;
	
	std::vector<AllResult> AttackAct(int Index, XMINT2 Pos);

	std::string m_Name; // 名前

public:
	bool m_MyTurn = false;
	bool m_Next = false;
	Enemy();
	~Enemy();

	void Initialize();
	void Finalize();

	void StartTurn();

	void SetMap(Map *Map);
	void SetPlayer(Player *player);

	void FindRouteToPlayer();

	void Update() override;
	void Draw() override;

	void Death();
};

