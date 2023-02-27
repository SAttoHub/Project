#include "SceneBase.h"

void SceneBase::FadeStart(int _Time, bool _isSceneChange)
{
	if (!m_isOut) {
		m_isOut = true;
		m_isIn = false;
		m_isSceneChange = _isSceneChange;
		m_Timer = 0;
		m_EndTimer = _Time;
	}
}

void SceneBase::FadeInStart(int _Time, bool _isSceneChange)
{
	if (!m_isIn) {
		m_isIn = true;
		m_isOut = false;
		m_isSceneChange = _isSceneChange;
		m_Timer = 0;
		m_EndTimer = _Time;
	}
}

void SceneBase::SceneTimeUpdate()
{
	if (m_isOut) {
		if (m_EndTimer > m_Timer) {
			m_Timer++;
		}
		else {
			if (m_isSceneChange) {
				m_IsNext = true;
			}
			m_isOut = false;
			m_Timer = 0;
		}
	}
	else if (m_isIn) {
		if (m_EndTimer > m_Timer) {
			m_Timer++;
		}
		else {
			m_isIn = false;
			m_Timer = 0;
		}
	}
}

void SceneBase::FadeDraw()
{
	if (m_isOut) {
		Fade::Instance()->DrawNormalFade(m_Timer, m_EndTimer);
	}
	else if(m_isIn) {
		Fade::Instance()->DrawNormalFadeIn(m_Timer, m_EndTimer);
	}
}
