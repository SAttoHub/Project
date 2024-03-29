#pragma once
#include <windows.h>

class FPS {
	double MIN_FREAM_TIME;
	LARGE_INTEGER NowTime; //今の時間
	LARGE_INTEGER OldTime; //前の時間
	LARGE_INTEGER TimeFreq; //周波数
public:
	FPS(float MAX_FPS_Value);
	~FPS();
	double FrameTime;
	static float fps;
	static float Maxfps;

	void FPS_Update();
	void ChangeFPS(float MAX_FPS_Value);
};

