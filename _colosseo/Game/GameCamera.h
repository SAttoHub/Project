#pragma once
#include "..\Camera.h"
#include "..\Engine\CommonMathIncludes.h"
#include "..\Singleton.h"

#include "TargetPoint.h"

#include "..\Engine\/Common\/ease.h"

//-------------------------------------------------------------------------------------------------------------
// カメラモード
//-------------------------------------------------------------------------------------------------------------
enum CameraMode {
	CM_Debug,	// 通常俯瞰カメラ 
	CM_Game		// ゲームシーンカメラ
};

//-------------------------------------------------------------------------------------------------------------
// ゲームシーンカメラ管理class
//-------------------------------------------------------------------------------------------------------------
class GameCamera : public Singleton<GameCamera> {
private:
	friend class Singleton<GameCamera>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
public:
	CameraMode Mode;
	XMFLOAT3 DebugCamPos;

	float ShakePower;
	int ShakeTimer;
	int ShakeEnd;
	XMFLOAT3 ShakeValue;
public:
	void Initialize();
	void Update();
	void Finalize();

	void DebugCameraUpdate();
	void GameCameraUpdate();

	void Shake(float Power, int Time);

private:
	struct PosData {
		XMFLOAT3 Position;
		float Range; // 注視点からのXZの距離
		float MarkRange;
		float AgoRange;
		float Angle; // 1,0,0を正面とした注視点からの角度
		float MarkAngle;
		float AgoAngle;
		float Height; // 中視点からの高さ
		float MarkHeight;
		float AgoHeight;
		int NowFlame[3];  // [0] : Range  /  [1]  : Angle  /  [2] : Height
		int EndFlame[3];
		bool CanChange[3];
	};
	PosData m_PosData;

	struct TargetData {
		XMFLOAT3 Target;
		XMFLOAT3 MarkTarget;
		XMFLOAT3 AgoTarget;
		int NowFlame;
		int EndFlame;
		bool CanChangeTarget;
	};
	TargetData m_TargetData;
public:
	const int DEFAULT_FLAME_TIME = 5;
	void Positioning(float Range, float Angle, float Height, int Flame);
	void ChangeRange(float Range, int Flame);
	void ChangeAngle(float Angle, int Flame);
	void ChangeHeight(float Height, int Flame);
	void Targeting(XMFLOAT3 Target, int Flame);
	void TargetingInstantly(XMFLOAT3 Target);
	float GetAngle() { return m_PosData.MarkAngle; }
};

