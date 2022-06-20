#pragma once
#include "Camera.h"
#include "DirectXMathExpansion.h"
#include "Singleton.h"

#include "StaticCameraPos.h"
#include "TargetPoint.h"

enum CameraMode {
	CM_Debug, // í èÌòÎ·’ÉJÉÅÉâ 
	CM_Game
};

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
};

