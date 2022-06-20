#include "window.h"

Window::Window(int X, int Y, int Width, int Height, WNDCLASSEX WindowData, DWORD Style) :
    X(X),
    Y(Y),
    Width(Width),
    Height(Height),
    WindowData(WindowData),
    Style(Style)
{
    MakeWindow();
}

Window::~Window() {
    UnregisterClass(WindowData.lpszClassName, WindowData.hInstance);
}

LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    //ウィンドウが破棄されたとき
    if (msg == WM_DESTROY)
    {
        //OSに対してアプリケーション終了を伝える
        PostQuitMessage(0);
        return 0;
    }
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    //規定の処理を行う
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::MakeWindow(void) {
    //ウィンドウの登録
    RegisterClassEx(&WindowData);
    //ウィンドウの生成
    WinHandle = CreateWindow(
        WindowData.lpszClassName,
        TEXT("HA-RI-TE"),
        Style,
        X,
        Y,
        Width,
        Height,
        NULL,
        NULL,
        WindowData.hInstance,
        NULL
    );
    
    //WindowData.hIcon = (HICON)LoadImage(          // アイコン
    //    NULL, (LPCWSTR)"ICON.ico", IMAGE_ICON,
    //    0, 0, LR_SHARED | LR_LOADFROMFILE
    //);
   // HICON LoadIcon(WindowData, (LPCTSTR)"蛇イナマイト");// リソース名の文字列またはリソース識別子

    //ウィンドウのリサイズ
    RECT window_rect;
    // ウィンドウサイズを取得
    GetWindowRect(WinHandle, &window_rect);
    RECT client_rect;
    GetClientRect(WinHandle, &client_rect);
    // フレームサイズ算出
    int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
    int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);
    // リサイズ用サイズの算出
    int resize_width = frame_size_x + Width;
    int resize_height = frame_size_y + Height;
    // ウィンドウサイズ更新
    SetWindowPos(
        // ウィンドウハンドル
        WinHandle,
        // 配置順序のハンドル
        NULL,
        // 表示座標X
        CW_USEDEFAULT,
        // 表示座標Y
        CW_USEDEFAULT,
        // リサイズ横幅		
        resize_width,
        // リサイズ縦幅
        resize_height,
        // SWP_NOMOVE => 位置変更なし
        SWP_NOMOVE);
}
