#include "NoviceAttackEffect.h"
#include "../Engine/Primitive/PrimitiveFunc.h"
#include "GameCamera.h"

#include "../Engine/Light/LightGroup.h"

void NoviceAttackEffect::SetStartAndEnd(XMFLOAT3 _Start, XMFLOAT3 _End)
{
	m_Start = _Start;
	m_End = _End;
	LightGroup::Ptr->SetPointLightPos(8, m_Start);
}

void NoviceAttackEffect::Start(int _Timer, int _EndTime)
{
	m_Timer = 0;
	m_EndTime = _EndTime;
	m_Size = 2.0f;
	m_isEnd = 0;
	LightGroup::Ptr->SetPointLightActive(8, true);
	LightGroup::Ptr->SetPointLightColor(8, ColorConvert(XMFLOAT3(255, 140, 0)));
	LightGroup::Ptr->SetPointLightAtten(8, XMFLOAT3(0.95f, 0.95f, 0.95f));
}

void NoviceAttackEffect::Update()
{
	// タイマーを進める
	TimerUpdate();
	if (m_isEnd) {
		LightGroup::Ptr->SetPointLightActive(8, false);
		LightGroup::Ptr->SetPointLightPos(8, XMFLOAT3(10000.0f, 0.0f, 1000.0f));
		return;
	}

	// エフェクトを動かす
	m_Target.x = Ease::EaseFunc(EaseName::Linear, m_Start.x, m_End.x, float(m_EndTime / 2), float(m_EndTime));
	m_Target.z = Ease::EaseFunc(EaseName::Linear, m_Start.z, m_End.z, float(m_EndTime / 2), float(m_EndTime));
	m_Target.x = m_Start.x;
	m_Target.y = m_Start.y;
	m_Target.z = m_Start.z;
	//float Height = 10.0f;
	//m_Target.y = m_Start.y + Height * sinf(((float(m_Timer) / float(m_EndTime)) * 180.0f * M_PI_F) / 180.0f);

	/*float hoge = 0.95f * sinf(((float(m_Timer) / float(m_EndTime)) * 180.0f * M_PI_F) / 180.0f);
	hoge = 1.0f - hoge;
	LightGroup::Ptr->SetPointLightAtten(8, XMFLOAT3(hoge, hoge, hoge));*/

	// モデル座標XZとカメラ座標XZ
	XMFLOAT2 modelPosXZ = XMFLOAT2(m_Start.x, m_Start.z);
	XMFLOAT2 cameraPosXZ = XMFLOAT2(Camera::eye.x, Camera::eye.z);
	// モデル→カメラベクトルを正規化する
	XMFLOAT2 norm = Normalize2D(modelPosXZ - cameraPosXZ);

	XMFLOAT3 LightPos = m_Target;
	LightPos.x += 2.0f * norm.x;
	LightPos.z += 2.0f * norm.y;
	LightPos.y += 0.5f;

	// ~~~10 光源点灯
	// 30~~~ 光源消灯
	if (m_Timer < 6) {
		float hoge = 0.95f * sinf(((float(m_Timer) / float(5)) * 90.0f * M_PI_F) / 180.0f);
		hoge = 1.0f - hoge;
		LightGroup::Ptr->SetPointLightAtten(8, XMFLOAT3(hoge, hoge, hoge));
	}
	else if (m_Timer > 34) {
		float hoge = 0.95f * sinf(((float(m_Timer - 35) / float(m_EndTime - 35)) * 90.0f * M_PI_F) / 180.0f);
		hoge = 0.05f + hoge;
		LightGroup::Ptr->SetPointLightAtten(8, XMFLOAT3(hoge, hoge, hoge));
		// ライトを浮かす
		LightPos.y += -0.5f * (m_Timer - m_EndTime);
	}

	// ライトの位置を決定
	LightGroup::Ptr->SetPointLightPos(8, LightPos);

	// カメラのターゲットをエフェクトにする
	GameCamera::Instance()->TargetingCoercion(m_Target, 1);
	// 演出中はカメラを固定しておく
	GameCamera::Instance()->CameraFixedNowFlame(10);
}

void NoviceAttackEffect::Draw()
{
	if (m_isEnd || m_Timer == 0) {
		return;
	}
	//DrawIcosahedron(m_Target, m_Size, ColorConvert(XMFLOAT4(0xff, 0x49, 0x01, 0xbf)), true);
}
