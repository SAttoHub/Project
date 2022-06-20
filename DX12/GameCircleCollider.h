#pragma once
#include "GameBaseCollider.h"
#include "CollisionPrimitive.h"

class GameCircleCollider : public GameBaseCollider, public Circle
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	GameCircleCollider(XMFLOAT2 *BasisPosition, XMFLOAT2 center = { 0,0 }, float radius = 1.0f, std::string tag = "None") :
		Center(center),
		Radius(radius)
	{
		// 基準点ポインタの更新
		Position = BasisPosition;
		Center = *Position;
		// 円のメンバ変数を更新
		Circle::center = Center;
		Circle::radius = Radius;
		// タグの設定
		GameBaseCollider::Tag = tag;
		// 円形状をセット
		shapeType = COL_SHAPE_CIRCLE;
	}

	~GameCircleCollider() {
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
	/// 描画　円のDrawは作ってないので機能しません
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 円の半径を変更する
	/// </summary>
	/// <param name="radius">半径</param>
	inline void SetRadius(float radius) { this->Radius = radius; }

private:
	XMFLOAT2 Center;
	float Radius;
};

