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
    //�E�B���h�E���j�����ꂽ�Ƃ�
    if (msg == WM_DESTROY)
    {
        //OS�ɑ΂��ăA�v���P�[�V�����I����`����
        PostQuitMessage(0);
        return 0;
    }
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    //�K��̏������s��
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::MakeWindow(void) {
    //�E�B���h�E�̓o�^
    RegisterClassEx(&WindowData);
    //�E�B���h�E�̐���
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
    
    //WindowData.hIcon = (HICON)LoadImage(          // �A�C�R��
    //    NULL, (LPCWSTR)"ICON.ico", IMAGE_ICON,
    //    0, 0, LR_SHARED | LR_LOADFROMFILE
    //);
   // HICON LoadIcon(WindowData, (LPCTSTR)"�փC�i�}�C�g");// ���\�[�X���̕�����܂��̓��\�[�X���ʎq

    //�E�B���h�E�̃��T�C�Y
    RECT window_rect;
    // �E�B���h�E�T�C�Y���擾
    GetWindowRect(WinHandle, &window_rect);
    RECT client_rect;
    GetClientRect(WinHandle, &client_rect);
    // �t���[���T�C�Y�Z�o
    int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
    int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);
    // ���T�C�Y�p�T�C�Y�̎Z�o
    int resize_width = frame_size_x + Width;
    int resize_height = frame_size_y + Height;
    // �E�B���h�E�T�C�Y�X�V
    SetWindowPos(
        // �E�B���h�E�n���h��
        WinHandle,
        // �z�u�����̃n���h��
        NULL,
        // �\�����WX
        CW_USEDEFAULT,
        // �\�����WY
        CW_USEDEFAULT,
        // ���T�C�Y����		
        resize_width,
        // ���T�C�Y�c��
        resize_height,
        // SWP_NOMOVE => �ʒu�ύX�Ȃ�
        SWP_NOMOVE);
}
