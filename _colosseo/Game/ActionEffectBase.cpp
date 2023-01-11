#include "ActionEffectBase.h"

void ActionEffectBase::TimerUpdate()
{
	// タイマーを進める・終わっていたら終了フラグを建てる
	if (m_Timer < m_EndTime) {
		m_Timer++;
	}
	else {
		m_isEnd = true;
	}
}
