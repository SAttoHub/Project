#pragma once
#include "Audience.h"

//-------------------------------------------------------------------------------------------------------------
// 観客管理class
//-------------------------------------------------------------------------------------------------------------
class Audiences {
private:
	const int MAX_AUDIENCE = 250;
	std::vector<Audience> m_Audience;

	Map *pMap = nullptr;

	int Timer = 0;
	int Hoge = 0; // 入退場テスト用

	int GetRandIndex(); // SummonAudience関数内処理 地形にめり込まないインデックスを取得する

	// 
	void AudienceSummonAndDelete();
public:
	Audiences();
	~Audiences();

	void Init(Map* map);
	void Update();
	void Draw();
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();

	// 入場演出なし
	void SummonAudience(int Count);
	// 入場演出あり
	void SummonAudience2(int Count);
	void DeleteAllAudience();
};