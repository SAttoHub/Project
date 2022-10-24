#pragma once
#include "Audience.h"

class Audiences {
private:
	const int MAX_AUDIENCE = 250;
	std::vector<Audience> m_Audience;

	Map *pMap = nullptr;

	int Timer = 0;
	int Hoge = 0; // ���ޏ�e�X�g�p
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
	void SummonAudience2(int Count); // ���ꉉ�o����
	int GetRandIndex(); // SummonAudience�֐������� �n�`�ɂ߂荞�܂Ȃ��C���f�b�N�X���擾����
	void DeleteAllAudience();
};