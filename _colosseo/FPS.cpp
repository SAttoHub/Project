#include "FPS.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
float FPS::fps = 0;
float FPS::Maxfps = 0;
//FPS�\���p

//#include <tchar.h>
//#include <sstream>
/*
fps = (fps * 0.99f) + (0.01f / FrameTime); //����fps���v�Z
#ifdef _DEBUG
	// �f�o�b�O�p(�f�o�b�K��FSP�o��)
#ifdef UNICODE
std::wstringstream stream;
#else
std::stringstream stream;
#endif
stream << fps << " FPS" << std::endl;

OutputDebugString(stream.str().c_str());
#endif // _DEBUG
*/



FPS::FPS(float MAX_FPS_Value) :
	MIN_FREAM_TIME(1.0f / (double)MAX_FPS_Value),
	FrameTime(0)
{
	//���x���擾����
	if (QueryPerformanceFrequency(&TimeFreq) == FALSE) {
	}
	//����v�Z�p�Ɍ��ݎ��Ԃ�O�t���[���̎��ԂƂ��Ď擾
	QueryPerformanceCounter(&OldTime);
	//�g��Ȃ����ǔO�̂��ߏ�����
	QueryPerformanceCounter(&NowTime);
	fps = 0;
	Maxfps = MAX_FPS_Value;
}

FPS::~FPS() {
}

void FPS::FPS_Update() {
	//���̎��Ԃ��擾
	QueryPerformanceCounter(&NowTime);
	//(���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	FrameTime = static_cast<double>(NowTime.QuadPart - OldTime.QuadPart) / static_cast<double>(TimeFreq.QuadPart);

	//���Ԃɗ]�T������
	if (FrameTime < MIN_FREAM_TIME) {
		//�~���b�ɕϊ�
		DWORD SleepTime = static_cast<DWORD>((MIN_FREAM_TIME - FrameTime) * 1000);

		timeBeginPeriod(1); //���x���グ��
		Sleep(SleepTime);   //�Q��
		timeEndPeriod(1);   //�߂�

		return;
	}

	//�o�ߎ��Ԃ�0���傫����
	if (FrameTime > 0.0) {
		fps = (fps * 0.99f) + (0.01f / float(FrameTime)); //����fps���v�Z
	}
	//���̎��Ԃ�O�̎��ԂɊi�[
	OldTime = NowTime;
}

void FPS::ChangeFPS(float MAX_FPS_Value) {
	MIN_FREAM_TIME = 1.0f / (double)MAX_FPS_Value;
	Maxfps = MAX_FPS_Value;
}
