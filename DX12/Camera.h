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
	Constant, //�Œ�
	NormalMove, //����(�H)
	TargetRange, //�����_�̂܂�����]����
	FreeMouse, //�}�E�X�Ŏ��R�ɉ�]�ł���
	Fps,
	PlayerLookDown
};

class Camera {
	static __int8 Mode;
	static int width, height;
	//�}�E�X����
	static XMFLOAT2 AskNormalizeCursorMoveDistance(); //�}�E�X�̈ړ����������߂Đ��K������

	static float distance;
	/*FPS*/
	static const float MAX_ANGLE_SPEED;
	static void SetTargetPointToAngle();
	/*---*/
public:
	static void CursorSetSenter();
	//�J���������_����
	static XMFLOAT3 angle;//�J������]�p
	static XMFLOAT3 radian;//�J������]�p(���W�A��)
	static XMVECTOR AxisX;
	static XMVECTOR AxisY;
	static XMVECTOR AxisZ;
	static XMMATRIX matBillboard;

	Camera(Window *Win);
	~Camera(){}

	static XMFLOAT3 eye; //���_���W
	static XMFLOAT3 oldeye; //���_���W
	static XMFLOAT3 target; //�����_���W
	static XMFLOAT3 up; //������x�N�g��
	static XMMATRIX matView; //�r���[�ϊ��s��
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
	/// �X�N���[�����W�����[���h���W�ɕϊ�
	/// </summary>
	/// <param name="ScreenPos">�X�N���[�����W</param>
	/// <param name="Z">Z���W</param>
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

