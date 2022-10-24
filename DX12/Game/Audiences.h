#pragma once
#include "Audience.h"

class Audiences {
private:
	const int MAX_AUDIENCE = 250;
	std::vector<Audience> m_Audience;

	Map *pMap = nullptr;

	int Timer = 0;
	int Hoge = 0; // 入退場テスト用
public:
	Audiences();
	~Audiences();

	void Init(Map* map);
	void Update();
	void Draw();
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();

	void SummonAudience(int Count);
	void SummonAudience2(int Count); // 入場演出あり
	int GetRandIndex(); // SummonAudience関数内処理 地形にめり込まないインデックスを取得する
	void DeleteAllAudience();
};