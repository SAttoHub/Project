#pragma once
#include <string>
#include "..\..\Engine\CommonMathIncludes.h"
#include "..\..\Engine\Primitive\PrimitiveFunc.h"
#include "..\..\Engine\Common\Colors\ColorFuncs.h"

enum GameCollisionShapeType
{
	COL_SHAPE_UNKNOWN = -1,		// 未設定
	COL_SHAPE_CIRCLE,			// 円
	COL_SHAPE_BOX,				// 箱
	COL_SHAPE_POINT,			// 点

	COL_3D_SHAPE_RAY,
	COL_3D_SHAPE_MOUSERAY,
	COL_3D_SHAPE_CUBE,
	COL_3D_SHAPE_PLANE
};

// 衝突判定拡張用 Class　これを継承したオブジェクトに対して衝突した時にコライダーを取得できる
class ColliderProp {
public:
	virtual ~ColliderProp() {}
};

class GameBaseCollider {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	GameBaseCollider() = default;
	virtual ~GameBaseCollider() = default;

	inline void SetBasisPosition(XMFLOAT2 *Position) {
		this->Position = Position;
	}

	inline XMFLOAT2 *GetBasisPosition() {
		return Position;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	inline GameCollisionShapeType GetShapeType() { return shapeType; }
	inline std::string GetTag() { return Tag; }

	/// <summary>
	/// 衝突判定関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const GameCollisionShapeType &info);

protected:
	// 基点座標
	XMFLOAT2 *Position = nullptr;
	// 形状タイプ
	GameCollisionShapeType shapeType = COL_SHAPE_UNKNOWN;

public:
	//タグ
	std::string Tag = "None";

	// 衝突判定拡張
	// コライダーを持つオブジェクトの親クラスのポインタ
	ColliderProp *Base;
	// コライダーを持つオブジェクトの親クラスのポインタをセットする
	void SetBaseProp(ColliderProp *Base) {
		this->Base = Base;
	}
};

