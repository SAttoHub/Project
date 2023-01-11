#include "MagicAttackEffect.h"
#include "../Engine/Primitive/PrimitiveFunc.h"
#include "GameCamera.h"

#include "../Engine/Light/LightGroup.h"

void MagicAttackEffect::SetStartAndEnd(XMFLOAT3 _Start, XMFLOAT3 _End)
{
	m_Start = _Start;
	m_End = _End;
	LightGroup::Ptr->SetPointLightPos(8, m_Start);
}

void MagicAttackEffect::Start(int _Timer, int _EndTime)
{
	m_Timer = 0;
	m_EndTime = _EndTime;
	m_Size = 2.0f;
	m_isEnd = 0;
	LightGroup::Ptr->SetPointLightActive(8, true);
	LightGroup::Ptr->SetPointLightColor(8, ColorConvert(XMFLOAT3(0xa2, 0x60, 0xbf)));
	LightGroup::Ptr->SetPointLightAtten(8, XMFLOAT3(0.03f, 0.03f, 0.03f));
}

void MagicAttackEffect::Update()
{
	// タイマーを進める
	TimerUpdate();
	if (m_isEnd) {
		LightGroup::Ptr->SetPointLightActive(8, false);
		LightGroup::Ptr->SetPointLightPos(8, XMFLOAT3(10000.0f, 0.0f, 1000.0f));
		return;
	}
	// エフェクトを動かす
	m_Target.x = Ease::EaseFunc(EaseName::Linear, m_Start.x, m_End.x, float(m_Timer), float(m_EndTime));
	m_Target.z = Ease::EaseFunc(EaseName::Linear, m_Start.z, m_End.z, float(m_Timer), float(m_EndTime));

	float Height = 10.0f;
	m_Target.y = m_Start.y + Height * sinf(((float(m_Timer) / float(m_EndTime)) * 180.0f * M_PI_F) / 180.0f);

	LightGroup::Ptr->SetPointLightPos(8, m_Target);

	// カメラのターゲットをエフェクトにする
	GameCamera::Instance()->TargetingCoercion(m_Target, 1);
	// 演出中はカメラを固定しておく
	GameCamera::Instance()->CameraFixedNowFlame(10);
}

void MagicAttackEffect::Draw()
{
	if (m_isEnd || m_Timer == 0) {
		return;
	}
	DrawIcosahedron(m_Target, m_Size, ColorConvert(XMFLOAT4(0xa2, 0x60, 0xbf, 0xbf)), true);
}
