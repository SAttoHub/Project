#include "FPS.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
float FPS::fps = 0;
float FPS::Maxfps = 0;
//FPS表示用

//#include <tchar.h>
//#include <sstream>
/*
fps = (fps * 0.99f) + (0.01f / FrameTime); //平均fpsを計算
#ifdef _DEBUG
	// デバッグ用(デバッガにFSP出す)
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
	//精度を取得する
	if (QueryPerformanceFrequency(&TimeFreq) == FALSE) {
	}
	//初回計算用に現在時間を前フレームの時間として取得
	QueryPerformanceCounter(&OldTime);
	//使わないけど念のため初期化
	QueryPerformanceCounter(&NowTime);
	fps = 0;
	Maxfps = MAX_FPS_Value;
}

FPS::~FPS() {
}

void FPS::FPS_Update() {
	//今の時間を取得
	QueryPerformanceCounter(&NowTime);
	//(今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	FrameTime = static_cast<double>(NowTime.QuadPart - OldTime.QuadPart) / static_cast<double>(TimeFreq.QuadPart);

	//時間に余裕がある
	if (FrameTime < MIN_FREAM_TIME) {
		//ミリ秒に変換
		DWORD SleepTime = static_cast<DWORD>((MIN_FREAM_TIME - FrameTime) * 1000);

		timeBeginPeriod(1); //精度を上げる
		Sleep(SleepTime);   //寝る
		timeEndPeriod(1);   //戻す

		return;
	}

	//経過時間が0より大きい時
	if (FrameTime > 0.0) {
		fps = (fps * 0.99f) + (0.01f / float(FrameTime)); //平均fpsを計算
	}
	//今の時間を前の時間に格納
	OldTime = NowTime;
}

void FPS::ChangeFPS(float MAX_FPS_Value) {
	MIN_FREAM_TIME = 1.0f / (double)MAX_FPS_Value;
	Maxfps = MAX_FPS_Value;
}
