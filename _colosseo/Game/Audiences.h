#pragma once
#include "Audience.h"
#include <vector>

//-------------------------------------------------------------------------------------------------------------
// �ϋq�Ǘ�class
//-------------------------------------------------------------------------------------------------------------
class Audiences {
private:
	const int MAX_AUDIENCE = 250;
	std::vector<Audience> m_Audience;

	Map *pMap = nullptr;
	InstanceObjectsData* model;

	int Timer = 0;
	int Hoge = 0; // ���ޏ�e�X�g�p

	int GetRandIndex(); // SummonAudience�֐������� �n�`�ɂ߂荞�܂Ȃ��C���f�b�N�X���擾����

	// 
	void AudienceSummonAndDelete();

	int Tex = 0;
public:
	Audiences();
	~Audiences();

	void Init(Map* map);
	void Update();
	void Draw();
	void ShadowDraw();
	void DepthDraw();
	void DOFDepthDraw();

	// ���ꉉ�o�Ȃ�
	void SummonAudience(int Count);
	// ���ꉉ�o����
	void SummonAudience2(int Count);
	void DeleteAllAudience();
};