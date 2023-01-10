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
	AC_Attack,
	AC_Charge	// 溜める(スタミナを回復して即終了)
};

//-------------------------------------------------------------------------------------------------------------
// 敵
//-------------------------------------------------------------------------------------------------------------
class Enemy : public Charactor
{
	Player *pPlayer = nullptr;
	AstarResults m_Route = AstarResults(); // ルート保存用


protected:
	// --------ターン開始時
	struct StartUp {
		// 移動可能コスト
		int m_MoveCost = 1;
	};
	StartUp m_StartUp;
	//-------------
	
private:
	int m_CoolTime = 0;
	// 移動可能コスト
	int m_MoveCost;
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

		int m_Range = 1;			// 射程
		bool m_IsLinear = false;	// 攻撃が直線的かどうか

		/// <param name="Name"> 技名 </param>
		/// <param name="Damage"> ダメージ </param>
		/// <param name="Cost"> 使用コスト </param>
		/// <param name="KnockBackPower"> ノックバック量 </param>
		/// <param name="Abn"> 与える状態異常 </param>
		/// <param name="AbnTurn"> 状態異常のターン </param>
		Enemy_Attack_Data(std::string Name, int Damage, int Cost, int KnockBackPower, Abnormality Abn, int AbnTurn) :
			m_Name(Name), m_Damage(Damage), m_Cost(Cost), m_KnockBackPower(KnockBackPower), m_Abn(Abn), m_AbnTurn(AbnTurn) {};

		void SetAttackRange(int _Range, bool _IsLinear) {
			m_Range = _Range;
			m_IsLinear = _IsLinear;
		}
	};
	std::vector<Enemy_Attack_Data> m_Attack_Datas;
	
	std::vector<AllResult> AttackAct(int Index, XMINT2 Pos);

	/// <summary>
	/// 攻撃が当たるかどうか。当たるなら攻撃可能としてtrueを返す
	/// </summary>
	/// <param name="MapPos">判定時の自分の位置</param>
	/// <param name="_Attack_Data"></param>
	/// <returns></returns>
	bool AttackHitsTheTarget(XMINT2 MapPos, const Enemy_Attack_Data &_Attack_Data);

	// 全ての行動に対して　攻撃可能なスタミナが残っているか・攻撃が当たるか 当たるならインデックスを、当たらないなら -1 を返す
	int CanAttack_AttackHitsTheTarget(XMINT2 MapPos);



	// ターン開始時行動判定
	void StartTurnWhitchAct();
	bool m_GenocideFlag = false; // プレイヤーを倒せる行動ができる場合にtrueになる
	// 移動しながら攻撃判定
	void Act();
	// ターンを終了する
	int TurnEndTimer = 20;
	bool TurnEndFlag = false;
	void TurnEnd();

	std::string m_Name; // 名前
	std::string m_JobName = ""; // 名前
	bool m_isBoss = false; // ボス敵フラグ

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

