#include "Charactor.h"
#include "..\Camera.h"
#include "..\DrawStrings.h"

void Charactor::SetDir()
{
	// オブジェクトの回転角度から正面を求める
	float Angle = Smath::ToRoundAngle(model->rotation.y);
	XMFLOAT2 FrontVec = Smath::AngleToDirectionVector(Angle);

	// モデル座標XZとカメラ座標XZ
	XMFLOAT2 modelPosXZ = XMFLOAT2(model->position.x, model->position.z);
	XMFLOAT2 cameraPosXZ = XMFLOAT2(Camera::eye.x, Camera::eye.z);
	// モデル→カメラベクトルを正規化する
	XMFLOAT2 norm = Normalize2D(modelPosXZ - cameraPosXZ);
	float cross = CalcAngle(FrontVec, norm);
	//　モデルから見てカメラが右側ならcrossに-1をかける
	if (Smath::GetDirection(modelPosXZ, FrontVec, cameraPosXZ)) {
		cross *= -1;
	}
	// 計算用補正
	cross = cross + 180.0f;
	cross /= 45.0f;

	if (cross < 0.5f) m_Dir = Chara_Dir::BACK;
	else if (cross < 1.5f) m_Dir = Chara_Dir::BACKRINGHT;
	else if (cross < 2.5f) m_Dir = Chara_Dir::RINGHT;
	else if (cross < 3.5f) m_Dir = Chara_Dir::FRONTRINGHT;
	else if (cross < 4.5f) m_Dir = Chara_Dir::FRONT;
	else if (cross < 5.5f) m_Dir = Chara_Dir::FRONTLEFT;
	else if (cross < 6.5f) m_Dir = Chara_Dir::LEFT;
	else if (cross < 7.5f) m_Dir = Chara_Dir::BACKLEFT;
	else m_Dir = Chara_Dir::BACK;

	//DrawStrings::Instance()->DrawFormatString(XMFLOAT2(5, 5), 32, XMFLOAT4(1, 1, 1, 1), "cross%f", cross);


}

Charactor::Charactor()
{
	/*m_MapPos = { 0,0 };
	m_HP = 1;

	int modelData = LoadModelOBJ("Charactor", "mob");
	model = DirectX3dObject::CreateObject(GetModelData(modelData),
		XMFLOAT3(0, 0, 0), FBXSHADER);*/
	m_Def = 0;
}

Charactor::~Charactor()
{
}

void Charactor::ShadowDraw()
{
	ShadowDepthDrawobject3d(model);
}

void Charactor::DepthDraw()
{
	DepthDrawobject3d(model);
}

void Charactor::DOFDepthDraw()
{
	DOFDepthDrawobject3d(model);
}
