#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "..\Engine\Objects\3DObject.h"
#include "..\DrawStrings.h"

#include "ActionManager.h"
#include "GameCamera.h"

//----------------------------------------------------------------
// リアクション一覧
//----------------------------------------------------------------
enum Reactions {
	Reac_None,			// リアクションなし
	Reac_Damage,		// ダメージ
	Reac_Death,			// 死亡
	Reac_Death_End		// 死亡終了後の削除待機
};

class Map;

//-------------------------------------------------------------------------------------------------------------
// キャラクター基底class
//-------------------------------------------------------------------------------------------------------------
class Charactor
{
#pragma region エイリアス

public:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

#pragma endregion

#pragma region コンストラクタ・デストラクタ

//----------------------------------------------------------------
// コンストラクタ・デストラクタ
//----------------------------------------------------------------

public:
	/// <summary>
	/// 基底コンストラクタ
	/// </summary>
	Charactor();

	/// <summary>
	/// 基底デストラクタ
	/// </summary>
	~Charactor();

#pragma endregion

#pragma region モデル・リソースデータ

//----------------------------------------------------------------
// モデル・リソースデータ
//----------------------------------------------------------------

protected:
	// キャラクターモデルポインタ
	Object3d* m_pModel = nullptr;

	// キャラクターテクスチャ
	int m_Image[8] = { 0 };

#pragma endregion

#pragma region 位置・マップ関連

//----------------------------------------------------------------
// 位置・マップ関連
//----------------------------------------------------------------

protected:
	// マップクラスのポインタ
	Map* m_pMap = nullptr;

	// 現在のマップ座標
	XMINT2 m_MapPos = XMINT2();

public:
	/// <summary>
	/// マップ座標を上書き
	/// </summary>
	/// <param name="_MapPos"> マップ座標 </param>
	void SetMapPos(XMINT2 _MapPos) { m_MapPos = _MapPos; }

	/// <summary>
	/// 現在のマップ座標を取得
	/// </summary>
	/// <returns></returns>
	XMINT2 GetMapPos() { return m_MapPos; }

	/// <summary>
	/// 現在のマップ座標のワールド座標を取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetWorldPos();

	/// <summary>
	/// モデルの現在位置を取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetModelPos() { return m_pModel->position; }

#pragma endregion

#pragma region 向き関連

//----------------------------------------------------------------
// 向き関連
//----------------------------------------------------------------

protected:
	/*------------------------------------------------
						BACK
			BACKRINGHT			BACKLEFT
	RINGHT									LEFT
			FRONTRINGHT			FRONTLEFT
						FRONT
	------------------------------------------------*/
	enum class Chara_Dir {
		FRONT,
		FRONTRINGHT,
		RINGHT,
		BACKRINGHT,
		BACK,
		BACKLEFT,
		LEFT,
		FRONTLEFT
	};

	// 現在向いている方向
	Chara_Dir m_NowDir = Chara_Dir::FRONT;

	// カメラから見たキャラの方向
	Chara_Dir m_DirFromCamera = Chara_Dir::FRONT;

public:
	/// <summary>
	/// 向きを変更する
	/// </summary>
	/// <param name="_Dir"> 向き </param>
	void SetDir(Chara_Dir _Dir) { m_NowDir = _Dir; }

	/// <summary>
	/// カメラから見た向きを更新する
	/// </summary>
	virtual void SetDirFromCamera();

#pragma endregion

#pragma region ステータス関連

//----------------------------------------------------------------
// ステータス関連
//----------------------------------------------------------------

protected:
	// 現在HP
	int m_HP = 1;
	// 最大HP
	int m_MaxHP = 1;
	// 防御力
	int m_Def = 0;
	// スタミナ？
	int m_Stamina = 5;
	// 現在の状態異常
	Abnormality m_Abn = Abnormality::None;

public:
	/// <summary>
	/// 現在のHPを取得
	/// </summary>
	/// <returns></returns>
	int GetHP() { return m_HP; }

	/// <summary>
	/// 現在の最大HPを取得
	/// </summary>
	/// <returns></returns>
	int GetMaxHP() { return m_MaxHP; }

	/// <summary>
	/// 防御力を取得
	/// </summary>
	/// <returns></returns>
	int GetDef() { return m_Def; }

	/// <summary>
	/// 現在のスタミナを取得
	/// </summary>
	/// <returns></returns>
	int GetStamina() { return m_Stamina; }

	/// <summary>
	/// 現在の状態異常を取得
	/// </summary>
	/// <returns></returns>
	Abnormality GetAbn() { return m_Abn; }

	/// <summary>
	/// 状態異常をセット
	/// </summary>
	/// <param name="_Abn">状態異常</param>
	void SetAbn(Abnormality _Abn) { m_Abn = _Abn; }

	/// <summary>
	/// _Valuw分のダメージを受ける
	/// </summary>
	/// <param name="_Value">ダメージ量</param>
	void Damage(int _Value) { m_HP -= _Value; };

	/// <summary>
	/// _Valuw分のダメージを受ける
	/// </summary>
	/// <param name="_Value">ダメージ量</param>
	void DamageShake(int _Value) { 
		m_HP -= _Value;
		GameCamera::Instance()->Shake(5, 14);
	};

	/// <summary>
	/// 現在生存しているか
	/// </summary>
	/// <returns> true : 生存   false : 死亡</returns>
	bool isAlive() { return m_HP > 0 ? true : false; }

	/// <summary>
	/// スタミナを使用する。スタミナが足りない場合は使用せず false を返す
	/// </summary>
	/// <param name="_Val">使用する量</param>
	/// <returns> true : スタミナが足りている   false : スタミナが足りていない</returns>
	bool UseStamina(int _Val) {
		if (m_Stamina < _Val) {
			return false;
		}
		m_Stamina -= _Val;
		return true;
	}

#pragma endregion

#pragma region 待ち時間関連

//----------------------------------------------------------------
// 待ち時間関連
//----------------------------------------------------------------

protected:
	// 移動時のカメラ待ち時間
	static const int WAIT_TIMER_VALUE_MOVE = 5;

	// カメラのターゲットを自分に変更する時のカメラ待ち時間
	static const int WAIT_TIMER_VALUE = 20;

	// 待機フレーム数( 30 なら 30 -= 1 していき、0になるまで待機 )
	int m_WaitTimer = 0;

public:
	/// <summary>
	/// 待ち時間をセット
	/// </summary>
	/// <param name="_Value">待機フレーム数</param>
	void Wait(int _Value) { m_WaitTimer = _Value; }

	/// <summary>
	/// 待機時間更新
	/// </summary>
	void WaitUpdate();

	/// <summary>
	/// 現在待機中かを取得
	/// </summary>
	/// <returns> true : 待機中  false : 待機中ではない </returns>
	bool isWait();

#pragma endregion

#pragma region リアクション関連

//----------------------------------------------------------------
// リアクション関連
//----------------------------------------------------------------

protected:
	// 現在のリアクション状態
	Reactions m_NowReaction = Reactions::Reac_None;
private:
	// ダメージリアクションで動く距離
	static const float DAMAGE_REACTION_OFFSET_RANGE;

	// ダメージリアクションの総フレーム数
	static const int DAMAGE_REACTION_TIME;

	// [一時保存用]　現在実行中のリアクションの総フレーム数
	int m_ReactionTimeMax = 0;

	// [一時保存用]　リアクションさせた側の座標
	XMINT2 m_ReactionMakeDir = XMINT2();

	// [一時保存用]　リアクション時の方向
	XMINT2 m_ReactionDoDir = XMINT2();

	/// <summary>
	/// させた側とする側の座標を使ってリアクションの方向を取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 OffsetDirGet();

	/// <summary>
	/// カメラの座標を使ってリアクションの方向を取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 CameraOffsetDirGet();

protected:
	// [一時保存用] リアクションの方向
	XMFLOAT3 m_ReactionOffset = XMFLOAT3();

	/// <summary>
	/// リアクション状態の更新
	/// </summary>
	void ReactionUpdate();

private:
	/// <summary>
	/// ダメージリアクション更新
	/// </summary>
	void DamageReactionUpdate();

	/// <summary>
	/// 死亡リアクション更新
	/// </summary>
	void DeathReactionUpdate();

public:
	/// <summary>
	/// ダメージリアクションをさせる
	/// </summary>
	/// <param name="_ReactionMakeDir">攻撃する側のマップ座標</param>
	/// <param name="_ReactionDoDir">攻撃される側のマップ座標</param>
	void DamageReaction(XMINT2 _ReactionMakeDir, XMINT2 _ReactionDoDir);

	/// <summary>
	/// 現在のリアクション状態を取得
	/// </summary>
	/// <returns></returns>
	Reactions GetReaction() { return m_NowReaction; }

#pragma endregion

#pragma region その他関数等

public:
	/// <summary>
	/// 仮想更新関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// カメラのターゲットを自分に変更する
	/// </summary>
	/// <param name="isWait"> カメラ移動完了まで待つか </param>
	/// /// <param name="WaitTime"> 待ち時間 </param>
	void CameraTargetOnMe(bool _isWait, int _WaitTime = WAIT_TIMER_VALUE);

#pragma endregion

#pragma region 描画関数

	/// <summary>
	/// 通常描画仮想関数
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// シャドウマップ用ライト視点描画
	/// </summary>
	void ShadowDraw();

	/// <summary>
	/// 深度描画
	/// </summary>
	void DepthDraw();

	/// <summary>
	/// DOF用深度描画
	/// </summary>
	void DOFDepthDraw();

#pragma endregion
};

