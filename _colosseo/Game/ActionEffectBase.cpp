#include "ActionEffectBase.h"

void ActionEffectBase::TimerUpdate()
{
	// �^�C�}�[��i�߂�E�I����Ă�����I���t���O�����Ă�
	if (m_Timer < m_EndTime) {
		m_Timer++;
	}
	else {
		m_isEnd = true;
	}
}
