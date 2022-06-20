#pragma once
#include <windows.h>

class FPS {
	double MIN_FREAM_TIME;
	double FrameTime;
	LARGE_INTEGER NowTime; //¡‚ÌŠÔ
	LARGE_INTEGER OldTime; //‘O‚ÌŠÔ
	LARGE_INTEGER TimeFreq; //ü”g”
public:
	FPS(float MAX_FPS_Value);
	~FPS();

	static float fps;
	static float Maxfps;

	void FPS_Update();
	void ChangeFPS(float MAX_FPS_Value);
};

