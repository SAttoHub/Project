#pragma once
#include <windows.h>
#include "imgui/imgui_impl_win32.h"

//デフォルトの画面のサイズ
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

class Window {
public:
	HWND WinHandle; //ウィンドウハンドル
	WNDCLASSEX WindowData; //ウィンドウのデータ
	int X, Y, Width, Height;
	DWORD Style; //ウィンドウスタイル

	 //ウィンドウプロシージャ
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//ウィンドウの作成
	void MakeWindow(void);

	Window(
		int X = 0,
		int Y = 0,
		int Width = WINDOW_WIDTH,
		int Height = WINDOW_HEIGHT,
		WNDCLASSEX WindowData = {
		sizeof(WNDCLASSEX),		// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,	// クラスのスタイル
		(WNDPROC)WindowProcedure,		// ウィンドウプロシージャ
		0,				// 補助メモリ
		0,				// 補助メモリ
		GetModuleHandle(0),			// このアプリのインスタンスハンドル
		(HICON)LoadImage(          // アイコン
		NULL, L"Resource/ICON.ico", IMAGE_ICON,
		0, 0, LR_SHARED | LR_LOADFROMFILE),// アイコン画像
		//LoadIcon(NULL, TEXT("KITTY")),
		LoadCursor(NULL, IDC_ARROW),	// カーソル画像
		NULL,				// 背景ブラシ(背景色)
		NULL,				// メニュー名
		TEXT("Window"),		// クラス名							
		NULL// 小さいアイコン
		},
		DWORD Style = WS_OVERLAPPEDWINDOW // ウィンドウスタイル(デフォルトはメインウィンドウ)
	); //コンストラクタ
	~Window(); //デストラクタ

	//ウィンドウハンドルの取得
	HWND Get(void) const {
		return WinHandle;
	}
};

