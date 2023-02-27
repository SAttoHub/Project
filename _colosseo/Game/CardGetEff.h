#pragma once
#include "Cards.h"

class CardGetEff {
private:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

	// 入手したカード
	CardType m_CardType;
	// 経過時間
	int m_Timer;
	// 最大時間
	static const int MAX_TIME = 30;
	// 終了していたら建てる
	bool m_isEnd;

	// カード中心座標
	XMFLOAT2 m_CenterPos;
	// カードサイズ
	XMFLOAT2 m_CardSize;
	// 拡縮サイズ
	float m_Size;
public:
	CardGetEff(CardType _GetCardType);
	~CardGetEff();

	void Update();
	void Draw();

	// 終了したか
	bool GetEnd() { return m_isEnd; }
};