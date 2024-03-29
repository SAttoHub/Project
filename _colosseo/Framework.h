#pragma once
#include "window.h"
#include "FPS.h"
#pragma comment (lib, "winmm.lib")
#include "Engine\Objects\3DObject.h"
#include "DXAudio.h"
#include "ShaderManager.h"
#define WIN32_LEAN_AND_MEAN
#include <stdexcept>
#include <ctime>        // time
#include <cstdlib>      // srand,rand
#include "GameManager.h"
#include "fbxsdk.h"
#include "Engine\Objects\Loader\FbxLoader.h"
#include "Input.h"
#include "Engine\Light\LightGroup.h"
#include "Engine\Common\Funcs.h"

class Framework
{
protected:
	FPS *fps = nullptr;
	Window *win = nullptr;

	DirectXBase DXBase;
	DXAudio audio;
	ShaderManager Shader;
	PipelineManager *Pipeline = nullptr;
	Camera *Cam = nullptr;
	DirectX3dObject DX;
	TexManager Tex;
	Input input;
public:
	virtual void Initialize();
	virtual void Finalize();

	Window *GetWindow() { return win; }
};

