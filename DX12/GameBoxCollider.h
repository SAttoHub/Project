#pragma once
#include "GameBaseCollider.h"
#include "CollisionPrimitive.h"

class GameBoxCollider : public GameBaseCollider, public Box
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	GameBoxCollider(XMFLOAT2 *BasisPosition, XMFLOAT2 pos1 = { 0,0 }, XMFLOAT2 pos2 = { 0,0 }, std::string tag = "None") :
		Pos1(pos1),
		Pos2(pos2)
	{
		// 基準点ポインタの更新
		Position = BasisPosition;
		// 箱のメンバ変数を更新
		Box::pos1 = Pos1 + *Position;
		Box::pos2 = Pos2 + *Position;
		// タグの設定
		GameBaseCollider::Tag = tag;
		// 箱形状をセット
		shapeType = COL_SHAPE_BOX;
	}

	~GameBoxCollider(){
		if (Base != nullptr) {
			delete Base;
		}
		if(Position != nullptr){
			delete Position;
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 基準点を 0,0 として箱のサイズを変更する
	/// </summary>
	/// <param name="pos1">左上座標</param>
	/// <param name="pos2">右下座標</param>
	inline void SetBoxSize(XMFLOAT2 pos1, XMFLOAT2 pos2);

private:
	// 左上座標( 中心座標を基準に0としたもの) 移動分は自動で計算
	XMFLOAT2 Pos1;
	// 右下座標( 中心座標を基準に0としたもの) 移動分は自動で計算
	XMFLOAT2 Pos2;
};

