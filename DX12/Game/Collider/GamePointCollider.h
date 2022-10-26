#pragma once
#include "GameBaseCollider.h"
#include "..\..\CollisionPrimitive.h"

//-------------------------------------------------------------------------------------------------------------
// 点コライダー
//-------------------------------------------------------------------------------------------------------------
class GamePointCollider : public GameBaseCollider, public Point
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	GamePointCollider(XMFLOAT2 *BasisPosition, std::string tag = "None")
	{
		// 基準点ポインタの更新
		Position = BasisPosition;
		// 箱のメンバ変数を更新
		Point::pos = *Position;
		// タグの設定
		GameBaseCollider::Tag = tag;
		// 箱形状をセット
		shapeType = COL_SHAPE_POINT;
	}

	~GamePointCollider() {
		if (Base != nullptr) {
			delete Base;
		}
		if (Position != nullptr) {
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
};

