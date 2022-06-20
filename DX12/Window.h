#pragma once
#include <windows.h>
#include "imgui/imgui_impl_win32.h"

//�f�t�H���g�̉�ʂ̃T�C�Y
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

class Window {
public:
	HWND WinHandle; //�E�B���h�E�n���h��
	WNDCLASSEX WindowData; //�E�B���h�E�̃f�[�^
	int X, Y, Width, Height;
	DWORD Style; //�E�B���h�E�X�^�C��

	 //�E�B���h�E�v���V�[�W��
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//�E�B���h�E�̍쐬
	void MakeWindow(void);

	Window(
		int X = 0,
		int Y = 0,
		int Width = WINDOW_WIDTH,
		int Height = WINDOW_HEIGHT,
		WNDCLASSEX WindowData = {
		sizeof(WNDCLASSEX),		// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,	// �N���X�̃X�^�C��
		(WNDPROC)WindowProcedure,		// �E�B���h�E�v���V�[�W��
		0,				// �⏕������
		0,				// �⏕������
		GetModuleHandle(0),			// ���̃A�v���̃C���X�^���X�n���h��
		(HICON)LoadImage(          // �A�C�R��
		NULL, L"Resource/ICON.ico", IMAGE_ICON,
		0, 0, LR_SHARED | LR_LOADFROMFILE),// �A�C�R���摜
		//LoadIcon(NULL, TEXT("KITTY")),
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���摜
		NULL,				// �w�i�u���V(�w�i�F)
		NULL,				// ���j���[��
		TEXT("Window"),		// �N���X��							
		NULL// �������A�C�R��
		},
		DWORD Style = WS_OVERLAPPEDWINDOW // �E�B���h�E�X�^�C��(�f�t�H���g�̓��C���E�B���h�E)
	); //�R���X�g���N�^
	~Window(); //�f�X�g���N�^

	//�E�B���h�E�n���h���̎擾
	HWND Get(void) const {
		return WinHandle;
	}
};

