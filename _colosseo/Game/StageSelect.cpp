#include "StageSelect.h"
#include "Enemys.h"
#include "Player.h"

//StageSelect::StageSelect(Enemys* en, Player* pl)
//{
//	pPlayer = pl;
//	pEnemys = en;
//
//	Start();
//}

void StageSelect::Initialize(Enemys* en, Player* pl)
{
	pPlayer = pl;
	pEnemys = en;

	BattleImg = TexManager::LoadTexture("Resource/image/SL_Battle.png");
	BossImg = TexManager::LoadTexture("Resource/image/SL_Boss.png");
	StageSelectImg = TexManager::LoadTexture("Resource/image/StageSelect.png");

	Start();
	SetStagesData();
	m_isFirst = true;
}

void StageSelect::LoadTextStage(std::string FileName)
{
	/*--------- 仮コード --------*/
	m_Wave.Waves.clear();
	m_Wave.SetPLandENPtr(pEnemys, pPlayer);

	if (FileName == "1-1") {
		WaveData TestWave1;
		TestWave1.WaveName = "1";
		TestWave1.EnemysData.emplace_back(WaveEnemy(NoviceEn, XMINT2(4, 6)));
		TestWave1.EnemysData.emplace_back(WaveEnemy(MagicEn, XMINT2(8, 7)));
		//TestWave1.EnemysData.emplace_back(WaveEnemy(NoviceBoss, XMINT2(4, 7)));
		//TestWave1.EnemysData.emplace_back(WaveEnemy(MagicBoss, XMINT2(5, 7)));
		m_Wave.Waves.emplace_back(TestWave1);
	}
	else if(FileName == "Boss") {
		WaveData TestWave1;
		TestWave1.WaveName = "1";
		TestWave1.EnemysData.emplace_back(WaveEnemy(NoviceEn, XMINT2(4, 7)));
		TestWave1.EnemysData.emplace_back(WaveEnemy(MagicEn, XMINT2(5, 7)));
		WaveData TestWave2;
		TestWave2.WaveName = "2";
		TestWave2.EnemysData.emplace_back(WaveEnemy(NoviceEn, XMINT2(4, 7)));
		TestWave2.EnemysData.emplace_back(WaveEnemy(MagicEn, XMINT2(5, 7)));
		TestWave2.EnemysData.emplace_back(WaveEnemy(NoviceEn, XMINT2(6, 7)));
		m_Wave.Waves.emplace_back(TestWave1);
		m_Wave.Waves.emplace_back(TestWave2);
		WaveData TestWave3;
		TestWave3.WaveName = "Boss";
		TestWave3.EnemysData.emplace_back(WaveEnemy(NoviceBoss, XMINT2(4, 7)));
		TestWave3.EnemysData.emplace_back(WaveEnemy(MagicBoss, XMINT2(5, 7)));
		m_Wave.Waves.emplace_back(TestWave1);
	}
	else {
		WaveData TestWave1;
		TestWave1.WaveName = "1";
		TestWave1.EnemysData.emplace_back(WaveEnemy(NoviceEn, XMINT2(4, 7)));
		WaveData TestWave2;
		TestWave2.WaveName = "1";
		TestWave2.EnemysData.emplace_back(WaveEnemy(MagicEn, XMINT2(8, 7)));
		m_Wave.Waves.emplace_back(TestWave1);
		m_Wave.Waves.emplace_back(TestWave2);
	}
}

void StageSelect::SetStagesData()
{
	Stages[0][2] = StageData(StageType::Battle, "1-1");

	Stages[1][1] = StageData(StageType::Battle, "2");
	Stages[1][2] = StageData(StageType::Battle, "2");
	Stages[1][3] = StageData(StageType::Battle, "2");

	Stages[2][0] = StageData(StageType::Battle, "3");
	Stages[2][1] = StageData(StageType::Battle, "3");
	Stages[2][2] = StageData(StageType::Battle, "3");
	Stages[2][3] = StageData(StageType::Battle, "3");
	Stages[2][4] = StageData(StageType::Battle, "3");

	/*Stages[3][0] = StageData(StageType::Battle, "4");
	Stages[3][1] = StageData(StageType::Battle, "4");
	Stages[3][2] = StageData(StageType::Battle, "4");
	Stages[3][3] = StageData(StageType::Battle, "4");
	Stages[3][4] = StageData(StageType::Battle, "4");*/

	Stages[3][0] = StageData(StageType::Battle, "Boss");
	Stages[3][1] = StageData(StageType::Battle, "Boss");
	Stages[3][2] = StageData(StageType::Battle, "Boss");
	Stages[3][3] = StageData(StageType::Battle, "Boss");
	Stages[3][4] = StageData(StageType::Battle, "Boss");
}

void StageSelect::SelectStart()
{
	// 現在のステージを選択中に
	m_SelectStageNum = m_NowStageNum;
	m_isSelect = false;
}

void StageSelect::SelectUpdate()
{
	SelectMove();
	StageEnter();
}

void StageSelect::SelectMove()
{
	//  x----------------->
	//  y
	//  |
	//  V
	
	
	// 次のステージを選択する
	bool isMoveCursor = false;
	XMINT2 Prop = m_SelectStageNum;
	if (Input::isKeyTrigger(DIK_UP)) {
		m_SelectStageNum.y -= 1;
		isMoveCursor = true;
	}
	if (Input::isKeyTrigger(DIK_DOWN)) {
		m_SelectStageNum.y += 1;
		isMoveCursor = true;
	}
	if (Input::isKeyTrigger(DIK_LEFT)) {
		m_SelectStageNum.x -= 1;
		isMoveCursor = true;
	}
	if (Input::isKeyTrigger(DIK_RIGHT)) {
		m_SelectStageNum.x += 1;
		isMoveCursor = true;
	}

	// 前のステージの次でかつ真上のステージに隣接していないと選択不可能 かつ配列範囲内か判定
	// 選択できる
	if (((m_SelectStageNum.x == m_NowStageNum.x + 1) &&
		(m_SelectStageNum.y == m_NowStageNum.y || m_SelectStageNum.y == m_NowStageNum.y - 1 || m_SelectStageNum.y == m_NowStageNum.y + 1) &&
		m_SelectStageNum.x >= 0 && m_SelectStageNum.x < StageCountX && m_SelectStageNum.y >= 0 && m_SelectStageNum.y < StageCountY )||
		m_SelectStageNum.x == m_NowStageNum.x && m_SelectStageNum.y == m_NowStageNum.y) {

	}
	// 選択できない
	else {
		m_SelectStageNum = Prop;
	}
	// 最初のステージ選択限定処理
	if (m_isFirst) {
		if (m_SelectStageNum != m_NowStageNum) {
			m_SelectStageNum = Prop;
		}
	}
}

void StageSelect::StageEnter()
{
	// ステージ決定
	if (Input::isKeyTrigger(DIK_RETURN)) {
		LoadTextStage(Stages[m_SelectStageNum.x][m_SelectStageNum.y].m_StageName);
		m_NowStageNum = m_SelectStageNum;
		m_isSelect = true;

		// 一度決定したら最初のステージ選択ではなくなる
		m_isFirst = false;
	}
}

void StageSelect::Start()
{
	m_NowStageNum = XMINT2(0, 2);
	m_SelectStageNum = m_NowStageNum;
	m_isSelect = false;
}

void StageSelect::Draw()
{
	DrawBox(XMFLOAT2(0,0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
	XMFLOAT2 Center = XMFLOAT2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
	XMINT2 OffsetCount = XMINT2();
	XMFLOAT2 Offset = XMFLOAT2();

	XMFLOAT2 Size = XMFLOAT2(256.0f, 128.0f);

	const XMFLOAT4 WHITE_COL_L = ColorConvert(XMFLOAT4(0.0f, 0.0f, 0.0f, 255.0f));

	for (int x = 0; x < StageCountX; x++) {
		for (int y = 0; y < StageCountY; y++) {
			if (Stages[x][y].m_StageType == StageType::None) {
				continue;
			}
			OffsetCount = m_NowStageNum - XMINT2(x, y);
			// オフセット
			Offset.x = Center.x + Size.x * -OffsetCount.x;
			Offset.y = Center.y + Size.y * -OffsetCount.y;

			/*DrawBox(XMFLOAT2(Offset.x - Size.x / 2.0f, Offset.y - Size.y / 2.0f), 
				XMFLOAT2(Offset.x + Size.x / 2.0f, Offset.y + Size.y / 2.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));*/
			if (Stages[x][y].m_StageType == StageType::Battle) {
				if (Stages[x][y].m_StageName == "Boss") {
					DrawGraph(XMFLOAT2(Offset.x - Size.x / 2.0f, Offset.y - Size.y / 2.0f),
						XMFLOAT2(Offset.x + Size.x / 2.0f, Offset.y + Size.y / 2.0f), BossImg);
				}
				else {
					DrawGraph(XMFLOAT2(Offset.x - Size.x / 2.0f, Offset.y - Size.y / 2.0f),
						XMFLOAT2(Offset.x + Size.x / 2.0f, Offset.y + Size.y / 2.0f), BattleImg);
				}
			}

			// 最初のステージ選択限定処理
			if (m_isFirst) {
				if (x != m_NowStageNum.x || y != m_NowStageNum.y) {
					DrawGraph(XMFLOAT2(Offset.x - Size.x / 2.0f, Offset.y - Size.y / 2.0f),
						XMFLOAT2(Offset.x + Size.x / 2.0f, Offset.y + Size.y / 2.0f),
						TexManager::GetColor(XMFLOAT4(30.0f, 30.0f, 30.0f, 150.0f)));
				}
			}
			else if (!(((x == m_NowStageNum.x + 1) &&
				(y == m_NowStageNum.y || y == m_NowStageNum.y - 1 || y == m_NowStageNum.y + 1) &&
				x >= 0 && x < StageCountX && y >= 0 && y < StageCountY) ||
				x == m_NowStageNum.x && y == m_NowStageNum.y)) {
				DrawGraph(XMFLOAT2(Offset.x - Size.x / 2.0f, Offset.y - Size.y / 2.0f),
					XMFLOAT2(Offset.x + Size.x / 2.0f, Offset.y + Size.y / 2.0f),
					TexManager::GetColor(XMFLOAT4(30.0f, 30.0f, 30.0f, 150.0f)));
			}
			
			//DrawStrings::Instance()->DrawFormatString(Offset, 28, WHITE_COL_L, "%s", Stages[x][y].m_StageName);

			/*if (x % 2 == 0) {
				if (y % 2 == 0) {
					DrawBox(Offset - Size / 2.0f, Offset + Size / 2.0f, XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
				}
				else {
					DrawBox(Offset - Size / 2.0f, Offset + Size / 2.0f, XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
				}
			}
			else {
				if (y % 2 == 0) {
					DrawBox(Offset - Size / 2.0f, Offset + Size / 2.0f, XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
				}
				else {
					DrawBox(Offset - Size / 2.0f, Offset + Size / 2.0f, XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
				}
			}*/

		}
	}
	OffsetCount = m_NowStageNum - m_SelectStageNum;
	// オフセット
	Offset.x = Center.x + Size.x * -OffsetCount.x;
	Offset.y = Center.y + Size.y * -OffsetCount.y;
	DrawGraph(XMFLOAT2(Offset.x - Size.x / 2.0f, Offset.y - Size.y / 2.0f),
		XMFLOAT2(Offset.x + Size.x / 2.0f, Offset.y + Size.y / 2.0f), TexManager::GetColor(XMFLOAT4(255.0f, 255.0f, 0.0f, 100.0f)));

	DrawGraph(XMFLOAT2(0, 0), XMFLOAT2(WINDOW_WIDTH, WINDOW_HEIGHT), StageSelectImg);
}
