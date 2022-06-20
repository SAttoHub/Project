#pragma once
#include "DirectXBase.h"
#include <DirectXMath.h>
using namespace DirectX;
#include"Input.h"
#define _USE_MATH_DEFINES
#include "Smath.h"
using namespace Smath;

#define PLAYER_HEIGHT 5.0f

enum CameraMoveMode {
	Constant, //固定
	NormalMove, //普通(？)
	TargetRange, //注視点のまわりを回転する
	FreeMouse, //マウスで自由に回転できる
	Fps,
	PlayerLookDown
};

class Camera {
	static __int8 Mode;
	static int width, height;
	//マウス制御
	static XMFLOAT2 AskNormalizeCursorMoveDistance(); //マウスの移動距離を求めて正規化する

	static float distance;
	/*FPS*/
	static const float MAX_ANGLE_SPEED;
	static void SetTargetPointToAngle();
	/*---*/
public:
	static void CursorSetSenter();
	//カメラ注視点制御
	static XMFLOAT3 angle;//カメラ回転角
	static XMFLOAT3 radian;//カメラ回転角(ラジアン)
	static XMVECTOR AxisX;
	static XMVECTOR AxisY;
	static XMVECTOR AxisZ;
	static XMMATRIX matBillboard;

	Camera(Window *Win);
	~Camera(){}

	static XMFLOAT3 eye; //視点座標
	static XMFLOAT3 oldeye; //視点座標
	static XMFLOAT3 target; //注視点座標
	static XMFLOAT3 up; //上方向ベクトル
	static XMMATRIX matView; //ビュー変換行列
	static XMMATRIX matProjection; //
	static XMMATRIX matViewProjection;

	void Update();
	void NormalModeMove();

	static bool CursorResetFlag;
	static bool isCameraStop;

	static void SetCameraPos(XMFLOAT3 pos);

	static void SetCameraPos2(XMFLOAT3 pos) {
		oldeye = eye;
		eye = pos;
	};

	static void SetCameraPos3(XMFLOAT3 pos);


	/// <summary>
	/// スクリーン座標をワールド座標に変換
	/// </summary>
	/// <param name="ScreenPos">スクリーン座標</param>
	/// <param name="Z">Z座標</param>
	/// <returns></returns>
	static XMFLOAT3 ConvertScreenToWorld(XMFLOAT2 ScreenPos, float Z);

	static XMFLOAT2 ConvertWorldToScreen(XMFLOAT3 WorldPos);



	static void CursorChange(bool isCameraStop_) {
		/*isCameraStop = false;
		CursorSetSenter();
		isCameraStop = isCameraStop_;
		int c = ShowCursor(FALSE);
		if (!isCameraStop) {
			while (c >= 0) {
				c = ShowCursor(FALSE);
			}
		}
		else {
			while (c < 0) {
				c = ShowCursor(TRUE);
			}
		}*/
	}

	static void Targeting(XMFLOAT3 Target);

	static void DebugCamera();

	

	
};

