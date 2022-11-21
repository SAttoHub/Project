#pragma once
#include "3DObject.h"
#include "Sprite.h"
#include "DXAudio.h"
#include "Input.h"
#include "Window.h"

static class DxData {
	static DirectXBase DXBase;
	static DirectXImage DX;
	static Sprite SP;
	static Input input;
	static DXAudio audio;
public:
	//DxData(Window win) {
	//	DXBase.DirectXReset(&win);
	//	DX.DirectXTriangleReset(&win, &DXBase);
	//	SP.DirectXTriangleReset(&win, &DXBase);
	//	DX.SetBlend(1);
	//	SP.SetBlend(1);
	//	audio.XAudioInit();
	//	input.CreateInput(&win, &DXBase);
	//};

	//static void Init() {
	//	SP.LoadSpriteTex(0, L"Resource/images.png");
	//	SP.LoadSpriteTex(1, L"Resource/images2.png");

	//	SpriteData spd[2];
	//	spd[0].spritePosition = { 0,0,0,0 };
	//	spd[0].texNumber = 0;
	//	spd[1].spritePosition = { 100,0,0,0 };
	//	spd[1].texNumber = 1;
	//	spd[1].AdjustSizeTexture = true;
	//	SP.SpriteInit(&spd[0]);
	//	SP.SpriteInit(&spd[1]);
	//	SP.spriteChangeSize(&spd[1], 300, 300);
	//};

	//static void Update() {
	//	DXBase.ClearScreen();
	//	input.Update(&DXBase);
	//	DX.Move(&input);
	//}

	//static void Draw() {
	//	SP.SpriteDraw(&spd[0]);
	//	SP.SpriteDraw(&spd[1]);
	//	DXBase.ClearDepthBuffer(); //深度バッファのクリア
	//	DX.Draw(); //描画
	//}
};

