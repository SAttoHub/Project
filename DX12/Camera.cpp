#include "Camera.h"
__int8 Camera::Mode = 0;
int Camera::width, Camera::height;
const float Camera::MAX_ANGLE_SPEED = 50.0f;
float Camera::distance = 30.0f;
XMFLOAT3 Camera::angle{ 0,0,0 };//カメラ回転角
XMFLOAT3 Camera::radian{ 0,0,0 };//カメラ回転角(ラジアン)
XMVECTOR Camera::AxisX;
XMVECTOR Camera::AxisY;
XMVECTOR Camera::AxisZ;
XMMATRIX Camera::matBillboard;
XMFLOAT3 Camera::eye{ 0, 0, 0 }; //視点座標
XMFLOAT3 Camera::oldeye{ 0, 0, 0 }; //視点座標
XMFLOAT3 Camera::target{ 0, 0, 0 }; //注視点座標
XMFLOAT3 Camera::up{ 0, 1, 0 }; //上方向ベクトル
XMMATRIX Camera::matView; //ビュー変換行列
XMMATRIX Camera::matProjection;
XMMATRIX Camera::matProjection2;
XMMATRIX Camera::matViewProjection;
XMMATRIX Camera::matViewProjection2;
bool Camera::CursorResetFlag = true;
bool Camera::isCameraStop = true;


Camera::Camera(Window *Win) {
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f), //画角60度
		(float)WINDOW_WIDTH / WINDOW_HEIGHT, //アスペクト比
		0.1f, 1000.0f //前端、奥端
	);
	//matProjection = XMMatrixOrthographicLH(
	//	(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, //アスペクト比
	//	0.1f, 2000.0f //前端、奥端
	//);

	matProjection2 = XMMatrixOrthographicLH(
		(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, //アスペクト比
		0.1f, 2000.0f //前端、奥端
	);

	//マウスをウィンドウの中心に
	RECT rc;
	GetWindowRect(Win->WinHandle, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	CursorSetSenter();
}

void Camera::CursorSetSenter() {
	if (!isCameraStop) {
		//マウスカーソルの位置をスクリーンの中心に変更する
		SetCursorPos(width / 2, height / 2);
	}
}

XMFLOAT2 Camera::AskNormalizeCursorMoveDistance()
{
	XMFLOAT2 Anser;
	//マウスの座標を取得する
	POINT Mpos;
	GetCursorPos(&Mpos);
	//移動量を求める
	int MoveX = (width / 2) - (int)Mpos.x;
	int MoveY = (height / 2) - (int)Mpos.y;
	//移動量を画面幅・高さ分移動した場合が 1になるよう正規化する
	Anser.x = (float)MoveX / (width / 2.0f);
	Anser.y = (float)MoveY / (height / 2.0f);
	return Anser;
}

void Camera::SetTargetPointToAngle() {
	if (!isCameraStop) {
		XMFLOAT2 Nor = AskNormalizeCursorMoveDistance();
		//回転角度を求める(最大速度はMAX_ANGLE_SPEED)
		float RotaX = Nor.x * MAX_ANGLE_SPEED;
		float RotaY = Nor.y * MAX_ANGLE_SPEED;
		//角度を保存しておく
		angle.x -= RotaY;
		angle.y -= RotaX;
		angle.z = 0;
		//回転させる
		radian.x -= RotaY * M_PI_F / 180.0f;
		radian.y -= RotaX * M_PI_F / 180.0f;
		radian.z = 0;

		//上下回転制限
		if (radian.x > 1.5f) {
			radian.x = 1.5f;
		}
		else if (radian.x < -1.5f) {
			radian.x = -1.5f;
		}
	}
	else {
		radian = XMFLOAT3{ 0,0,0 };
	}
	/*VGet(SPos.x + Range * sin(HRotate) * cos(VRotate),
		SPos.y - Range * sin(VRotate),
		SPos.z + Range * cos(HRotate) * cos(VRotate))*/

	target.x = distance * sin(radian.y) * cos(radian.x) + eye.x;
	target.y = -distance * sin(radian.x) + eye.y;
	target.z = distance * cos(radian.y) * cos(radian.x) + eye.z;

	//ビュー変換
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	XMVECTOR eyepos = XMLoadFloat3(&eye);
	XMVECTOR targetpos = XMLoadFloat3(&target);
	XMVECTOR upVec = XMLoadFloat3(&up);
	AxisZ = XMVectorSubtract(targetpos, eyepos);
	assert(!XMVector3Equal(AxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(AxisZ));
	assert(!XMVector3Equal(upVec, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVec));
	AxisZ = XMVector3Normalize(AxisZ);
	AxisX = XMVector3Cross(upVec, AxisZ);
	AxisX = XMVector3Normalize(AxisX);
	AxisY = XMVector3Cross(AxisZ, AxisX);
	//ビルボード行列
	matBillboard.r[0] = Camera::AxisX;
	matBillboard.r[1] = Camera::AxisY;
	matBillboard.r[2] = Camera::AxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
	matViewProjection2 = matView * matProjection2;
}

void Camera::Update(){
	/*if (CursorResetFlag == true) {
		SetTargetPointToAngle();
		CursorSetSenter();
	}*/

}

void Camera::NormalModeMove() {

}

void Camera::Targeting(XMFLOAT3 Target) {
	target = Target;
	//target.y -= distance;

	/*radian.x = 1.57f;
	radian.y = 0;

	target.x = distance * sin(radian.y) * cos(radian.x) + eye.x;
	target.y = -distance * sin(radian.x) + eye.y;
	target.z = distance * cos(radian.y) * cos(radian.x) + eye.z;*/

	//ビュー変換
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	XMVECTOR eyepos = XMLoadFloat3(&eye);
	XMVECTOR targetpos = XMLoadFloat3(&target);
	XMVECTOR upVec = XMLoadFloat3(&up);
	AxisZ = XMVectorSubtract(targetpos, eyepos);
	assert(!XMVector3Equal(AxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(AxisZ));
	assert(!XMVector3Equal(upVec, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVec));
	AxisZ = XMVector3Normalize(AxisZ);
	AxisX = XMVector3Cross(upVec, AxisZ);
	AxisX = XMVector3Normalize(AxisX);
	AxisY = XMVector3Cross(AxisZ, AxisX);
	//ビルボード行列
	matBillboard.r[0] = Camera::AxisX;
	matBillboard.r[1] = Camera::AxisY;
	matBillboard.r[2] = Camera::AxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
	matViewProjection2 = matView * matProjection2;
}

void Camera::SetCameraPos(XMFLOAT3 pos) {
	oldeye = eye;
	eye = pos;
	SetTargetPointToAngle();
	CursorSetSenter();
}
void Camera::SetCameraPos3(XMFLOAT3 pos)
{
	oldeye = eye;
	eye = pos;
	
	if (!isCameraStop) {
		XMFLOAT2 Nor = AskNormalizeCursorMoveDistance();
		//回転角度を求める(最大速度はMAX_ANGLE_SPEED)
		float RotaX = Nor.x * MAX_ANGLE_SPEED;
		float RotaY = Nor.y * MAX_ANGLE_SPEED;
		//角度を保存しておく
		angle.x -= RotaY;
		angle.y -= RotaX;
		angle.z = 0;
		//回転させる
		radian.x -= RotaY * M_PI_F / 180.0f;
		radian.y -= RotaX * M_PI_F / 180.0f;
		radian.z = 0;

		//上下回転制限
		if (radian.x > 1.5f) {
			radian.x = 1.5f;
		}
		else if (radian.x < -1.5f) {
			radian.x = -1.5f;
		}
	}
	else {
		radian = XMFLOAT3{ 0,0,0 };
	}
	/*VGet(SPos.x + Range * sin(HRotate) * cos(VRotate),
		SPos.y - Range * sin(VRotate),
		SPos.z + Range * cos(HRotate) * cos(VRotate))*/

	//target.x = distance * sin(radian.y) * cos(radian.x) + eye.x;
	//target.y = -distance * sin(radian.x) + eye.y;
	//target.z = distance * cos(radian.y) * cos(radian.x) + eye.z;

	//ビュー変換
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	XMVECTOR eyepos = XMLoadFloat3(&eye);
	XMVECTOR targetpos = XMLoadFloat3(&target);
	XMVECTOR upVec = XMLoadFloat3(&up);
	AxisZ = XMVectorSubtract(targetpos, eyepos);
	assert(!XMVector3Equal(AxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(AxisZ));
	assert(!XMVector3Equal(upVec, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVec));
	AxisZ = XMVector3Normalize(AxisZ);
	AxisX = XMVector3Cross(upVec, AxisZ);
	AxisX = XMVector3Normalize(AxisX);
	AxisY = XMVector3Cross(AxisZ, AxisX);
	//ビルボード行列
	matBillboard.r[0] = Camera::AxisX;
	matBillboard.r[1] = Camera::AxisY;
	matBillboard.r[2] = Camera::AxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
	matViewProjection2 = matView * matProjection2;
}
;

void Camera::DebugCamera() {

}

XMFLOAT3 Camera::ConvertScreenToWorld(XMFLOAT2 ScreenPos, float Z) {
	XMMATRIX invView, invPrj, invViewPort;
	invView = XMMatrixInverse(nullptr, matView);
	invPrj = XMMatrixInverse(nullptr, matProjection);

	invViewPort = XMMatrixIdentity();
	invViewPort.r[0].m128_f32[0] = (float)WINDOW_WIDTH / 2.0f;
	invViewPort.r[1].m128_f32[1] = (float)-WINDOW_HEIGHT / 2.0f;
	invViewPort.r[3].m128_f32[0] = (float)WINDOW_WIDTH / 2.0f;
	invViewPort.r[3].m128_f32[1] = (float)WINDOW_HEIGHT / 2.0f;
	invViewPort = XMMatrixInverse(nullptr, invViewPort);

	XMVECTOR pos;
	pos = XMLoadFloat3(&XMFLOAT3(ScreenPos.x, ScreenPos.y, Z));
	pos = XMVector3Transform(pos, invViewPort);
	pos = XMVector3TransformCoord(pos, invPrj);
	pos = XMVector3Transform(pos, invView);

	XMFLOAT3 result;
	result.x = pos.m128_f32[0];
	result.y = pos.m128_f32[1];
	result.z = pos.m128_f32[2];

	return result;
}

XMFLOAT2 Camera::ConvertWorldToScreen(XMFLOAT3 WorldPos)
{
	// ビュー行列と射影行列の取得
	XMMATRIX view = Camera::matView;
	XMMATRIX proj = matProjection;

	// ビューポート行列（スクリーン行列）の作成
	float w = (float)WINDOW_WIDTH / 2.0f;
	float h = (float)WINDOW_HEIGHT / 2.0f;

	XMMATRIX viewport = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w , h , 0 , 1
	};

	XMVECTOR screenPos;
	XMVECTOR WorldPosV = XMLoadFloat3(&WorldPos);
	// ビュー変換とプロジェクション変換
	WorldPosV = VTransform(WorldPosV, view);
	WorldPosV = VTransform(WorldPosV, proj);

	// zで割って-1~1の範囲に収める
	WorldPosV.m128_f32[0] /= WorldPosV.m128_f32[2];
	WorldPosV.m128_f32[1] /= WorldPosV.m128_f32[2];
	WorldPosV.m128_f32[2] /= WorldPosV.m128_f32[2];
	// スクリーン変換
	screenPos = VTransform(WorldPosV, viewport);

	XMFLOAT2 result;
	XMStoreFloat2(&result, screenPos);

	return result;
}