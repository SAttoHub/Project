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
// カード種類
//-------------------------------------------------------------------------------------------------------------
enum class CardType {
	NONE,
	DEFAULT_ATTACK,
	DEFAULT_MOVE,
	NEXT_EN_JUMP
};

//-------------------------------------------------------------------------------------------------------------
// カード
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

	// 初期化共通
	void ConstructInitialize();
	// ポインタセット
	void SetPlEnPtr(Player* _pPlayer, Enemys* _pEnemys, Map* _pMap);

	// -----------------------------------
	// カードパラメータ
	// -----------------------------------
public:
	// 現在選択中のマス
	XMINT2 m_NowCheckChip = XMINT2();
	// 攻撃した場合のダメージ
	int m_PreDamage = 0;
	// フェーズ数
	int m_PheseCount = 3;


	// -----------------------------------
	// フェーズ管理用
	// -----------------------------------
public:
	bool m_PheseChange = false;
protected:
	// 待機タイマー
	int m_WaitTimer;
	// フェーズ開始時のトリガーフラグ
	bool m_PheseStartTriggerFlag;
	// フェーズ終了フラグ
	bool m_isPheseEnd;
	// 待機タイマーが0でかつフェーズ終了フラグが上がっている場合に true を返す
	// ついでに待機タイマーの更新も行う
	bool CanPhaseEnd();

	/// <summary>
	/// 現在のフェーズを _WaitFlame フレーム後に終了させる
	/// </summary>
	/// <param name="_WaitFlame">終了までのフレーム数</param>
	void PheseEndFunc(int _WaitFlame);

	// -----------------------------------
	// フェーズ管理用変数取得
	// -----------------------------------
public:


	// -----------------------------------
	// フェーズ内処理関数等
	// -----------------------------------
public:


	// -----------------------------------
	// 処理フェーズ1
	// -----------------------------------
	
	// フェーズ1開始時トリガー処理
	virtual void Phese1Start() = 0;
	// フェーズ1更新処理
	virtual void UpdatePhese1() = 0;
	// フェーズ1終了処理
	virtual void Phese1End() = 0;


	// -----------------------------------
	// 処理フェーズ2
	// -----------------------------------
	
	// フェーズ2開始時トリガー処理
	virtual void Phese2Start() = 0;
	// フェーズ2更新処理
	virtual void UpdatePhese2() = 0;
	// フェーズ2終了処理
	virtual void Phese2End() = 0;


	// -----------------------------------
	// 処理フェーズ3
	// -----------------------------------
	
	// フェーズ3開始時トリガー処理
	virtual void Phese3Start() = 0;
	// フェーズ3更新処理
	virtual void UpdatePhese3() = 0;
	// フェーズ3終了処理
	virtual void Phese3End() = 0;

	
	/// <summary>
	/// カード選択時トリガー処理
	/// </summary>
	/// /// <param name="RefPoint">選択可能マスを生成する基準のマップ座標</param>
	/// <param name="AddCanSelectChips">Chipsクラス</param>
	virtual void SelectTrigger(XMINT2 RefPoint, Cards* pCards) = 0;

	// 選択マスのセット
	virtual void SetSelectData(XMINT2 _Now, XMINT2 _Pre) = 0;
};

