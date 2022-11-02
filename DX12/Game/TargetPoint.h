#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Singleton.h"

//-------------------------------------------------------------------------------------------------------------
// 敵エネミー用ターゲットポイント
//-------------------------------------------------------------------------------------------------------------
class TargetPoint : public Singleton<TargetPoint> {
	friend class Singleton<TargetPoint>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	XMFLOAT3 position = XMFLOAT3(); // ターゲットポイントの位置

public:
	// ターゲットポイントの座標を取得
	XMFLOAT3 GetPos() { return position; }
	// ターゲットポイントの座標をセット
	void SetPos(XMFLOAT3 pos) { position = pos; }
};

// ターゲットポイントの座標を取得
static DirectX::XMFLOAT3 GetTargetPos() {
	return TargetPoint::Instance()->GetPos();
}

// ターゲットポイントの座標をセット
static void SetTargetPos(DirectX::XMFLOAT3 pos) {
	TargetPoint::Instance()->SetPos(pos);
}