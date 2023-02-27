#pragma once
#include "Card.h"

class Enemy;

class NormalAttackCard : public Card {

	int Timer = 0;
	Enemy* Pen;
public:
	// コンストラクタ
	NormalAttackCard();
	// デストラクタ
	~NormalAttackCard();

	// 描画
	void Draw(int index, int MaxIdx) override;
	
	// -----------------------------------
	// 処理フェーズ1
	// -----------------------------------

	// フェーズ1開始時トリガー処理
	void Phese1Start() override;
	// フェーズ1更新処理
	void UpdatePhese1() override;
	// フェーズ1終了処理
	void Phese1End() override;


	// -----------------------------------
	// 処理フェーズ2
	// -----------------------------------

	// フェーズ2開始時トリガー処理
	void Phese2Start() override;
	// フェーズ2更新処理
	void UpdatePhese2() override;
	// フェーズ2終了処理
	void Phese2End() override;


	// -----------------------------------
	// 処理フェーズ3
	// -----------------------------------

	// フェーズ3開始時トリガー処理
	void Phese3Start() override;
	// フェーズ3更新処理
	void UpdatePhese3() override;
	// フェーズ3終了処理
	void Phese3End() override;

	/// <summary>
	/// カード選択時トリガー処理
	/// </summary>
	/// /// <param name="RefPoint">選択可能マスを生成する基準のマップ座標</param>
	/// <param name="AddCanSelectChips">Chipsクラス</param>
	void SelectTrigger(XMINT2 RefPoint, Cards* pCards) override;


	// -----------------------------------
	// プレイヤー向き変更
	// -----------------------------------
	void ChangePlayerDir(Player* _pPlayer);


	// -----------------------------------
	// 処理用パラメータ類
	// -----------------------------------

	// 前回選択していたマス
	XMINT2 m_PreCheckChip = XMINT2();
	// 選択マスのセット
	void SetSelectData(XMINT2 _Now, XMINT2 _Pre) override;
};
