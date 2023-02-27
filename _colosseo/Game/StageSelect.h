#pragma once
#include <string>
#include "BattleWave.h"

class StageSelect {
	using WaveEnemy = BattleWave::WaveEnemy;
	using WaveData = BattleWave::WaveData;
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;

	Enemys* pEnemys = nullptr;
	Player* pPlayer = nullptr;

	// Wave�֘A
	BattleWave m_Wave;

public:
	/*StageSelect(Enemys* en, Player* pl);
	~StageSelect(){}*/
	void Initialize(Enemys* en, Player* pl);
	// �X�e�[�W�����w�肵�ă��[�h����
	void LoadTextStage(std::string FileName);

	BattleWave* GetWaveData() {
		return &m_Wave;
	}
	enum class StageType {
		None,
		Battle,
		Shop
	};
private:
	struct StageData {
		StageType m_StageType = StageType::None;
		std::string m_StageName = "None";
		StageData() {
			m_StageType = StageType::None;
			m_StageName = "None";
		}
		StageData(StageType Type, std::string Name) {
			m_StageType = Type;
			m_StageName = Name;
		}
	};
	int BattleImg = 0;
	int ShopImg = 0;
	int BossImg = 0;
	int StageSelectImg = 0;

	static const int StageCountX = 4;
	static const int StageCountY = 5;
	StageData Stages[StageCountX][StageCountY] = {};
	void SetStagesData();

	
	XMINT2 m_NowStageNum;
	XMINT2 m_SelectStageNum;
	
	// �ŏ��̃X�e�[�W�I�����t���O
	bool m_isFirst = true;
	void SelectMove();
	void StageEnter();

public:
	void SelectStart();
	void SelectUpdate();
	void Start();

	void Draw();

	bool m_isSelect = false;
	StageData GetNowStageData() {
		return Stages[m_NowStageNum.x][m_NowStageNum.y];
	}

	// ���̃X�e�[�W
	StageType m_NextStageType = StageType::None;

	// ������
	/*
public:
	// ���̃X�e�[�W�œ�����o���l
	int m_StageExp;
	*/
};