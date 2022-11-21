#pragma once
#include "window.h"

#include "DirectXBase.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <xinput.h>
#pragma comment(lib, "Xinput.lib")

#include "Engine\CommonMathIncludes.h"
using namespace Smath;
#include "CollisionPrimitive.h"
#include "Camera.h"

#pragma region API_Input�֘A

//-------------�}�E�X�֘A-------------//
#define API_MOUSE_LEFT          0x01     //�}�E�X���{�^��
#define API_MOUSE_RIGHT         0x02     //�}�E�X�E�{�^��
#define API_MOUSE_CENTER        0x04     //�}�E�X�����{�^��
#define API_MOUSE_BUTTON_1      0x05     //�}�E�X��1�g���{�^��
#define API_MOUSE_BUTTON_2      0x06     //�}�E�X��2�g���{�^��



//-------------�e���L�[�֘A-------------//
//---0�`9---///
#define API_KEY_NUMPAD_0        0x60     //0�L�[
#define API_KEY_NUMPAD_1        0x23     //1�L�[
#define API_KEY_NUMPAD_2        0x28     //2�L�[
#define API_KEY_NUMPAD_3        0x22     //3�L�[
#define API_KEY_NUMPAD_4        0x25     //4�L�[
#define API_KEY_NUMPAD_5        0x0C     //5�L�[
#define API_KEY_NUMPAD_6        0x27     //6�L�[
#define API_KEY_NUMPAD_7        0x24     //7�L�[
#define API_KEY_NUMPAD_8        0x26     //8�L�[
#define API_KEY_NUMPAD_9        0x21     //9�L�[
//---NUMLOCK 0�`9---///
#define API_KEY_NUMPAD_L_0      0x60     //NumLock && 0�L�[
#define API_KEY_NUMPAD_L_1      0x61     //NumLock && 1�L�[
#define API_KEY_NUMPAD_L_2      0x62     //NumLock && 2�L�[
#define API_KEY_NUMPAD_L_3      0x63     //NumLock && 3�L�[
#define API_KEY_NUMPAD_L_4      0x64     //NumLock && 4�L�[
#define API_KEY_NUMPAD_L_5      0x65     //NumLock && 5�L�[
#define API_KEY_NUMPAD_L_6      0x66     //NumLock && 6�L�[
#define API_KEY_NUMPAD_L_7      0x67     //NumLock && 7�L�[
#define API_KEY_NUMPAD_L_8      0x68     //NumLock && 8�L�[
#define API_KEY_NUMPAD_L_9      0x69     //NumLock && 9�L�[
//---  +  -  *  /  �����_  ---///
#define API_KEY_MULTI           0x6A     //�e���L�[�� * �L�[
#define API_KEY_ADD             0x6B     //�e���L�[�� + �L�[
#define API_KEY_SEPARATOR       0x6C     //�e���L�[�� Enter �L�[
#define API_KEY_SUB             0x6D     //�e���L�[�� - �L�[
#define API_KEY_DECIMAL         0x6E     //�e���L�[�� . (�����_)�L�[
#define API_KEY_DIVIDE          0x6F     //�e���L�[�� / �L�[
#define API_KEY_OFF_DECIMAL     0x2E     //NumLock�I�t�̃e���L�[[.]



//-------------0�`9-------------//
#define API_KEY_0               0x30     //0�L�[
#define API_KEY_1               0x31     //1�L�[
#define API_KEY_2               0x32     //2�L�[
#define API_KEY_3               0x33     //3�L�[
#define API_KEY_4               0x34     //4�L�[
#define API_KEY_5               0x35     //5�L�[
#define API_KEY_6               0x36     //6�L�[
#define API_KEY_7               0x37     //7�L�[
#define API_KEY_8               0x38     //8�L�[
#define API_KEY_9               0x39     //9�L�[
//-------------A�`Z�L�[�֘A-------------//
#define API_KEY_A               0x41     //A�L�[
#define API_KEY_B               0x42     //B�L�[
#define API_KEY_C               0x43     //C�L�[
#define API_KEY_D               0x44     //D�L�[
#define API_KEY_E               0x45     //E�L�[
#define API_KEY_F               0x46     //F�L�[
#define API_KEY_G               0x47     //G�L�[
#define API_KEY_H               0x48     //H�L�[
#define API_KEY_I               0x49     //I�L�[
#define API_KEY_J               0x4A     //J�L�[
#define API_KEY_K               0x4B     //K�L�[
#define API_KEY_L               0x4C     //L�L�[
#define API_KEY_M               0x4D     //M�L�[
#define API_KEY_N               0x4E     //N�L�[
#define API_KEY_O               0x4F     //O�L�[
#define API_KEY_P               0x50     //P�L�[
#define API_KEY_Q               0x51     //Q�L�[
#define API_KEY_R               0x52     //R�L�[
#define API_KEY_S               0x53     //S�L�[
#define API_KEY_T               0x54     //T�L�[
#define API_KEY_U               0x55     //U�L�[
#define API_KEY_V               0x56     //V�L�[
#define API_KEY_W               0x57     //W�L�[
#define API_KEY_X               0x58     //X�L�[
#define API_KEY_Y               0x59     //Y�L�[
#define API_KEY_Z               0x5A     //Z�L�[



//-------------���L�[-------------//
#define API_KEY_LEFT            0x25     //Left�L�[ || NumLock�I�t�̃e���L�[[4]
#define API_KEY_UP              0x26     //Up�L�[ || NumLock�I�t�̃e���L�[[8]
#define API_KEY_RIGHT           0x27     //Right�L�[ || NumLock�I�t�̃e���L�[[6]
#define API_KEY_DOWN            0x28     //Down�L�[ || NumLock�I�t�̃e���L�[[2]



//-------------�t�@���N�V�����L�[-------------//
#define API_KEY_F1              0x70     //F1�L�[
#define API_KEY_F2              0x71     //F2�L�[
#define API_KEY_F3              0x72     //F3�L�[
#define API_KEY_F4              0x73     //F4�L�[
#define API_KEY_F5              0x74     //F5�L�[
#define API_KEY_F6              0x75     //F6�L�[
#define API_KEY_F7              0x76     //F7�L�[
#define API_KEY_F8              0x77     //F8�L�[
#define API_KEY_F9              0x78     //F9�L�[
#define API_KEY_F10             0x79     //F10�L�[
#define API_KEY_F11             0x7A     //F11�L�[
#define API_KEY_F12             0x7B     //F12�L�[
#define API_KEY_F13             0x7C     //F13�L�[
#define API_KEY_F14             0x7D     //F14�L�[
#define API_KEY_F15             0x7E     //F15�L�[
#define API_KEY_F16             0x7F     //F16�L�[
#define API_KEY_F17             0x80     //F17�L�[
#define API_KEY_F18             0x81     //F18�L�[
#define API_KEY_F19             0x82     //F19�L�[
#define API_KEY_F20             0x83     //F20�L�[
#define API_KEY_F21             0x84     //F21�L�[
#define API_KEY_F22             0x85     //F22�L�[
#define API_KEY_F23             0x86     //F23�L�[
#define API_KEY_F24             0x87     //F24�L�[



//-------------���̑��̃L�[�{�[�h-------------//
#define API_KEY_BACK            0x08     //Backspace�L�[
#define API_KEY_TAB             0x09     //Tab�L�[
#define API_KEY_CLEAR           0x0C     //Clear�L�[ || NumLock�I�t�̃e���L�[[5]
#define API_KEY_ENTER           0x0D     //Enter�L�[ || �e���L�[Enter�L�[
#define API_KEY_SHIFT           0x10     //Shift�L�[
#define API_KEY_LSHIFT          0xA0     //��Shift�L�[
#define API_KEY_RSHIFT          0xA1     //�EShift�L�[
#define API_KEY_CONTROL         0x11     //Ctrl�L�[
#define API_KEY_LCONTROL        0xA2     //��Ctrl�L�[
#define API_KEY_RCONTROL        0xA3     //�ECtrl�L�[
#define API_KEY_MENU            0x12     //Alt�L�[
#define API_KEY_LMENU           0xA4     //��Alt�L�[
#define API_KEY_RMENU           0xA5     //�EAlt�L�[
#define API_KEY_PAUSE           0x13     //Pause�L�[
#define API_KEY_ESCAPE          0x1B     //Esc�L�[
#define API_KEY_SPACE           0x20     //Space�L�[
#define API_KEY_PGUP            0x21     //PageUp�L�[ || NumLock�I�t�̃e���L�[[9]
#define API_KEY_PGDN            0x22     //PageDown�L�[ || NumLock�I�t�̃e���L�[[3]
#define API_KEY_END             0x23     //End�L�[ || NumLock�I�t�̃e���L�[[1]
#define API_KEY_HOME            0x24     //Home�L�[ || NumLock�I�t�̃e���L�[[7]
#define API_KEY_SELECT          0x29     //Select�L�[
#define API_KEY_PRINT           0x2A     //Print�L�[
#define API_KEY_EXECUTE_        0x2B     //Execute�L�[ //�Ē�`���
#define API_KEY_SNAPSHOT        0x2C     //PrintScreen�L�[
#define API_KEY_INSERT          0x2D     //Insert�L�[ || NumLock�I�t�̃e���L�[[0]
#define API_KEY_DELETE          0x2E     //Delete�L�[ || NumLock�I�t�̃e���L�[[.]
#define API_KEY_HELP            0x2F     //Help�L�[
#define API_KEY_LWIN            0x5B     //��Windows�L�[
#define API_KEY_RWIN            0x5C     //�EWindows�L�[
#define API_KEY_APPS            0x5D     //�A�v���P�[�V�����L�[
#define API_KEY_SLEEP           0x5F     //�R���s���[�^�[�X���[�v�L�[
#define API_KEY_NUMLOCK         0x90     //NumLock�L�[
#define API_KEY_SCROLL          0x91     //ScrollLock�L�[
#define API_KEY_ATTN            0xF6     //Attn�L�[
#define API_KEY_CRSEL           0xF7     //CrSel�L�[
#define API_KEY_EXSEL           0xF8     //ExSel�L�[
#define API_KEY_EREOF           0xF9     //Erase EOF�L�[
#define API_KEY_PLAY            0xFA     //Play�L�[
#define API_KEY_Zoom            0xFB     //Zoom�L�[
#define API_KEY_PA1             0xFD     //PA1�L�[



//-------------INE�֘A-------------//
#define API_IME_KANA            0x15     //IME ���� ���[�h
#define API_IME_JUNJA           0x17     //IME Junja ���[�h
#define API_IME_FINAL           0x18     //IME �t�@�C�i�� ���[�h
#define API_IME_KANJI           0x19     //IME ���� ���[�h [Alt]+[���p�^�S�p]
#define API_IME_CONVERT         0x1C     //IME �ϊ� [�ϊ�]
#define API_IME_NONCONVERT      0x1D     //IME ���ϊ� [���ϊ�]
#define API_IME_ACCEPT          0x1E     //IME �g�p�\
#define API_IME_MODECHANGE      0x1F     //IME ���[�h�ύX�v��
#define API_IME_PROCESS         0xE5     //IME PROCESS�L�[



//-------------OEM�֘A-------------//
#define API_OEM_NULL_1          0x92     //OEM�ŗL �s��
#define API_OEM_NULL_2          0x93     //OEM�ŗL �s��
#define API_OEM_NULL_3          0x94     //OEM�ŗL �s��
#define API_OEM_NULL_4          0x95     //OEM�ŗL �s��
#define API_OEM_NULL_5          0x96     //OEM�ŗL �s��
#define API_OEM_MULTI           0xBA     // : * �L�[
#define API_OEM_COLON           0xBA     // : * �L�[
#define API_OEM_1               0xBA     // : * �L�[
#define API_OEM_PULS            0xBB     // ; + �L�[
#define API_OEM_SEMICOLON       0xBB     // ; + �L�[
#define API_OEM_CONMA           0xBC     // , < �L�[
#define API_OEM_SYOUNARI        0xBC     // , < �L�[
#define API_OEM_SUB             0xBD     // - = �L�[
#define API_OEM_SAME            0xBD     // - = �L�[
#define API_OEM_PERIOD          0xBE     // . > �L�[
#define API_OEM_DAINARI         0xBE     // . > �L�[
#define API_OEM_SLASH           0xBF     // / ? �L�[
#define API_OEM_QUESTION        0xBF     // / ? �L�[
#define API_OEM_2               0xBF     // / ? �L�[
#define API_OEM_ATMARK          0xC0     // @ ` �L�[
#define API_OEM_3               0xC0     // @ ` �L�[
#define API_OEM_4               0xDB     // [ { �L�[
#define API_OEM_5               0xDC     // \ | �L�[
#define API_OEM_6               0xDD     // ] } �L�[
#define API_OEM_7               0xDE     // ^ ~ �L�[
#define API_OEM_8               0xCF     //�s��
#define API_OEM_AX              0xE1     //OEM�ŗL �s��
#define API_OEM_102             0xE2     // �o�b�N�X���b�V�� _ �L�[
#define API_OEM_HELP            0xE3     //OEM�ŗL �s��
#define API_OEM_00              0xE4     //OEM�ŗL �s��
#define API_OEM_CLEAR           0xFE     //Clear�L�[
#define API_OEM_RESET           0xE9     //OEM�ŗL �s��
#define API_OEM_JUMP            0xEA     //OEM�ŗL �s��
#define API_OEM_PA1             0xEB     //OEM�ŗL �s��
#define API_OEM_PA2             0xEC     //OEM�ŗL �s��
#define API_OEM_PA3             0xED     //OEM�ŗL �s��
#define API_OEM_WSCTRL          0xEE     //OEM�ŗL �s��
#define API_OEM_CUSEL           0xEF     //OEM�ŗL �s��
#define API_OEM_ATTN            0xF0     //OEM�ŗL [�p�� CapsLock]
#define API_OEM_FINISH          0xF1     //OEM�ŗL �s��
#define API_OEM_COPY            0xF2     //OEM�ŗL [�J�^�J�i �Ђ炪��]
#define API_OEM_AUTO            0xF3     //OEM�ŗL [���p�^�S�p]
#define API_OEM_ENLW            0xF4     //OEM�ŗL �s��
#define API_OEM_BACKTAB         0xF5     //OEM�ŗL [Alt]+[�J�^�J�i �Ђ炪��]



//-------------�����L�[�֘A-------------//
#define API_KEY_CANCEL          0x03     //[Ctrl]+[Pause]  ||  [Ctrl]+[ScrollLock]
#define API_KEY_CAPITAL         0x14     //[Shift] && [�p�� CapsLock]



//-------------�u���E�U�֘A-------------//
#define API_BROWSER_BACK        0xA6     //�u���E�U�̖߂�L�[
#define API_BROWSER_FORWARD     0xA7     //�u���E�U�̐i�ރL�[
#define API_BROWSER_REFRESH     0xA8     //�u���E�U�̍X�V�L�[
#define API_BROWSER_STOP        0xA9     //�u���E�U�̒�~�L�[
#define API_BROWSER_SEARCH      0xAA     //�u���E�U�̌����L�[
#define API_BROWSER_FAVORITES   0xAB     //�u���E�U�̂��C�ɓ���L�[
#define API_BROWSER_HOME        0xAC     //�u���E�U�̊J�n����уz�[���L�[



//-------------�����֘A-------------//
#define API_VOLUME_MUTE         0xAD     //���ʃ~���[�g�L�[
#define API_VOLUME_DOWN         0xAE     //���ʃ_�E���L�[
#define API_VOLUME_UP           0xAF     //���ʃA�b�v�L�[



//-------------���f�B�A�֘A-------------//
#define API_MEDIA_NEXT_TRACK    0xB0     //���̃g���b�N�L�[
#define API_MEDIA_PREV_TRACK    0xB1     //�O�̃g���b�N�L�[
#define API_MEDIA_STOP          0xB2     //���f�B�A��~�L�[
#define API_MEDIA_PLAY_PAUSE    0xB3     //���f�B�A�Đ�/�ꎞ��~�L�[



//-------------LAUNCH�֘A-------------//
#define API_LAUNCH_MAIL         0xB4     //���[���N���L�[
#define API_LAUNCH_MEDIA_SELECT 0xB5   //���f�B�A�I���L�[
#define API_LAUNCH_APP1         0xB6     //�A�v���P�[�V����1�N���L�[
#define API_LAUNCH_APP2         0xB7     //�A�v���P�[�V����2�N���L�[



//-------------���̑�-------------//
#define API_OTHER_PACKET        0xE7     //�L�[�{�[�h�ȊO�̓��͎�i�Ɏg�p�����32 �r�b�g���z�L�[�l�̉��ʃ��[�h	
//-------------��-------------//
/*
0x07		����`
0x0A		�\���
0x0B		�\���
0x0E		����`
0x0F		����`
0x1A		����`
0x3A		����`
0x3B		����`
0x3C		����`
0x3D		����`
0x3E		����`
0x3F		����`
0x40		����`
0x88		��������
0x89		��������
0x8A		��������
0x8B		��������
0x8C		��������
0x8D		��������
0x8E		��������
0x8F		��������
0x97		��������
0x98		��������
0x99		��������
0x9A		��������
0x9B		��������
0x9C		��������
0x9D		��������
0x9E		��������
0x9F		��������
0xB8		�\���
0xB9		�\���
0xC1		�\���
0xC2		�\���
0xC3		�\���
0xC4		�\���
0xC5		�\���
0xC6		�\���
0xC7		�\���
0xC8		�\���
0xC9		�\���
0xCA		�\���
0xCB		�\���
0xCC		�\���
0xCD		�\���
0xCE		�\���
0xCF		�\���
0xD0		�\���
0xD1		�\���
0xD2		�\���
0xD3		�\���
0xD4		�\���
0xD5		�\���
0xD6		�\���
0xD7		�\���
0xD8		��������
0xD9		��������
0xDA		��������
0xE0		�\���
0xE8		��������
0xFC	VK_NONAME	�\���
*/

//WINAPIInput
class WinAPIInput {
private:
	static SHORT keys[256];
	static SHORT oldkeys[256];
public:

	static void Update()
	{
		for (int i = 0; i < 256; i++) {
			oldkeys[i] = keys[i];
			/* ���݂̃L�[�����擾 */
			keys[i] = GetAsyncKeyState(i);
		}
	}

	static bool isKey(int keyIndex)
	{
		return keys[keyIndex];
	}
	static bool oldkey(int keyIndex)
	{
		return oldkeys[keyIndex];
	}

	static bool isKeyTrigger(int keyIndex)
	{
		return keys[keyIndex] && !oldkeys[keyIndex];
	}
};

#pragma endregion


//DirectInput
//�}�E�X
enum MouseButton {
	M_LEFT,
	M_RIGHT,
	M_CENTER
};
//�Q�[���p�b�h
enum ButtonKind
{
	UpButton,
	DownButton,
	LeftButton,
	RightButton,
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonKindMax,
};
enum ButtonState
{
	ButtonStateNone,
	ButtonStateDown,
	ButtonStatePush,
	ButtonStateUp,
	ButtonStateMax,
};

#pragma region XInput
//-------------XInput-------------//
#pragma region wButtons
#define D_X_UP			XINPUT_GAMEPAD_DPAD_UP
#define D_X_DOWN		XINPUT_GAMEPAD_DPAD_DOWN
#define D_X_LEFT		XINPUT_GAMEPAD_DPAD_LEFT
#define D_X_RIGHT		XINPUT_GAMEPAD_DPAD_RIGHT
#define D_X_START		XINPUT_GAMEPAD_START
#define D_X_BACK		XINPUT_GAMEPAD_BACK
#define D_X_LTHUMB		XINPUT_GAMEPAD_LEFT_THUMB  //�X�e�B�b�N�̉�������?
#define D_X_RTHUMB		XINPUT_GAMEPAD_RIGHT_THUMB
#define D_X_LB			XINPUT_GAMEPAD_LEFT_SHOULDER
#define D_X_RB			XINPUT_GAMEPAD_RIGHT_SHOULDER
#define D_X_A			XINPUT_GAMEPAD_A
#define D_X_B			XINPUT_GAMEPAD_B
#define D_X_X			XINPUT_GAMEPAD_X
#define D_X_Y			XINPUT_GAMEPAD_Y
#pragma endregion

enum XInputButtonState {
	X_UP,
	X_DOWN,
	X_LEFT,
	X_RIGHT,
	X_START,
	X_BACK,
	X_LTHUMB,
	X_RTHUMB,
	X_LB,
	X_RB,
	X_LT,
	X_RT,
	X_A,
	X_B,
	X_X,
	X_Y,
	MAX_BUTTONS
};
enum StickTiltDirection {
	STICK_UP,
	STICK_DOWN,
	STICK_LEFT,
	STICK_RIGHT,
	STICK_UP_LEFT,
	STICK_UP_RIGHT,
	STICK_DOWN_LEFT,
	STICK_DOWN_RIGHT,
	STICK_TILT_MAX
};
//--------------------------------//
#pragma endregion

class Input {
#pragma region DirectInput
	struct EnumParameter {
		LPDIRECTINPUTDEVICE8 *_devGamePad;
		int FindCount;
	};
	// �Q�[���p�b�h�f�o�C�X�̍쐬
	static bool CreateGamePadDevice();
	static bool StartGamePadControl();
	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static bool SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device);
	static bool SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device);
	//�Q�[���p�b�h�̍X�V
	static void UpdateGamePad();
public:
	static IDirectInput8 *dinput;
	static IDirectInputDevice8 *devkeyborad;
	static IDirectInputDevice8 *devmouse;
	static LPDIRECTINPUTDEVICE8 devGamePad;

	static BYTE keys[256];
	static BYTE oldkeys[256];
	static DIMOUSESTATE2 mouse;
	static DIMOUSESTATE2 oldmouse;
	static XMFLOAT2 mousePos;
	static Ray mouseRay;
	struct MouseMove {
		LONG IX;
		LONG IY;
		LONG IZ;
		MouseMove() : IX(0), IY(0), IZ(0) {};
		MouseMove(LONG x, LONG y, LONG z) : IX(x), IY(y), IZ(z) {};
	};
	static ButtonState g_ButtonStates[ButtonKind::ButtonKindMax];

	static bool isConnectGamePad;

	//Input�̏�����
	static void Initialize(Window *win);

	//----------���͏�Ԃ̍X�V----------//

	static void Update();

	//----------�e���͏�Ԏ擾�֐��n----------//
#pragma region DirectInput���͏��
#pragma region �L�[�{�[�h
	//�L�[��������Ă��邩
	static bool isKey(int keyIndex) {
		if (keyIndex < 0 || keyIndex > 255)return false;
		return keys[keyIndex];
	}
	//�L�[�������ꂽ�u�Ԃ�
	static bool isKeyTrigger(int keyIndex){
		if (keyIndex < 0 || keyIndex > 255)return false;
		return keys[keyIndex] && !oldkeys[keyIndex];
	}
	//�L�[�𗣂����u�Ԃ�
	static bool isKeyRelease(int keyIndex) {
		if (keyIndex < 0 || keyIndex > 255)return false;
		return !keys[keyIndex] && oldkeys[keyIndex];
	}
#pragma endregion
#pragma region �}�E�X
	//�}�E�X�̃{�^����������Ă��邩
	static bool isMouse(int mouseIndex) {
		if (mouseIndex < 0 || mouseIndex > 255)return false;
		return mouse.rgbButtons[mouseIndex];
	}
	//�}�E�X�̃{�^���������ꂽ�u�Ԃ�
	static bool isMouseTrigger(int mouseIndex) {
		if (mouseIndex < 0 || mouseIndex > 255)return false;
		return mouse.rgbButtons[mouseIndex] && !oldmouse.rgbButtons[mouseIndex];
	}
	//�}�E�X�̃{�^���𗣂����u�Ԃ�
	static bool isMouseRelease(int mouseIndex) {
		if (mouseIndex < 0 || mouseIndex > 255)return false;
		return !mouse.rgbButtons[mouseIndex] && oldmouse.rgbButtons[mouseIndex];
	}
	//�}�E�X�̍��W���擾
	static XMFLOAT2 GetMousePos() {
		return mousePos;
	}
	//�}�E�X�̃��C���擾
	static Ray GetMouseRay();
	//�}�E�X�̈ړ��ʂ��擾
	static MouseMove GetMouseMove() {
		MouseMove tmp;
		tmp.IX = mouse.lX;
		tmp.IY = mouse.lY;
		tmp.IZ = mouse.lZ;
		return tmp;
	}
	//�z�C�[��
	static LONG GetMouseWheelMove() {
		LONG IZ = mouse.lZ;
		return IZ;
	}
#pragma endregion
#pragma region �R���g���[���[
	//�R���g���[���[�̃{�^����������Ă��邩
	static bool DirectIsButton(ButtonKind button) {
		if (g_ButtonStates[button] == ButtonState::ButtonStatePush) return true;
		return false;
	}
	//�R���g���[���[�̃{�^���������ꂽ�u�Ԃ�
	static bool DirectIsButtonTrigger(ButtonKind button) {
		if (g_ButtonStates[button] == ButtonState::ButtonStateUp) return true;
		return false;
	}
	//�R���g���[���[�̃{�^���𗣂����u�Ԃ�
	static bool DirectIsButtonRelease(ButtonKind button) {
		if (g_ButtonStates[button] == ButtonState::ButtonStateDown) return true;
		return false;
	}
#pragma endregion
#pragma endregion

#pragma endregion

#pragma region XInput
	//-------------XInput-------------//
private:
	//�R���g���[���[���F�����ꂽ���ǂ���
	static bool isConnectedXGamePad;
	//XInput�\����
	static XINPUT_STATE XGamePad;
	// �U��
	static XINPUT_VIBRATION vibration;
	static int vibrationFlame;
	static void UpdateXInput();
public:
	static bool PadButton[MAX_BUTTONS];
	static bool OldPadButton[MAX_BUTTONS];
	static SHORT LStick_X;
	static SHORT LStick_Y;
	static SHORT RStick_X;
	static SHORT RStick_Y;
	static int TiltDeadZoneL; //���X�e�B�b�N�̃f�b�h�]�[��
	static int TiltDeadZoneR; //�E�X�e�B�b�N�̃f�b�h�]�[��
	//----------�e���͏�Ԏ擾�֐��n----------//
#pragma region Buttons
	//�R���g���[���[�̃{�^����������Ă��邩
	static bool IsButton(XInputButtonState button) {
		return PadButton[button];
	}
	//�R���g���[���[�̃{�^���������ꂽ�u�Ԃ�
	static bool IsButtonTrigger(XInputButtonState button) {
		return PadButton[button] && !OldPadButton[button];
	}
	//�R���g���[���[�̃{�^���𗣂����u�Ԃ�
	static bool IsButtonRelease(XInputButtonState button) {
		return !PadButton[button] && OldPadButton[button];
	}
#pragma endregion
#pragma region LSticks
	/// <summary>
	/// ���X�e�B�b�N�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="DeadZone">�ύX����l : MAX 32767 : MIN 0</param>
	static void ChangeDeadZoneL(int DeadZone) {
		if (DeadZone > 32767)DeadZone = 32767;
		if (DeadZone < 0)DeadZone = 0;
		TiltDeadZoneL = DeadZone;
	}
	//�f�b�h�]�[���ȉ��Ȃ�0��Ԃ��A�����łȂ���΂��̂܂ܕԂ�
	static SHORT DeadZoneLInfluence(SHORT valuem, int DeadZone = TiltDeadZoneL) {
		if (valuem <  TiltDeadZoneL &&
			valuem > -TiltDeadZoneL) {
			return 0;
		}
		return valuem;
	}
	//�f�b�h�]�[�����Ȃ�false��Ԃ��A�����łȂ����true��Ԃ�
	static bool isInDeadZoneL() {
		if (LStick_X <  TiltDeadZoneL &&
			LStick_X > -TiltDeadZoneL &&
			LStick_Y <  TiltDeadZoneL &&
			LStick_Y > -TiltDeadZoneL) {
			return false;
		}
		return true;
	}
	/// <summary>
	/// ���X�e�B�b�N�̌X��X���擾����
	/// </summary>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneL</param>
	/// <returns></returns>
	static SHORT GetLStickTiltX(int DeadZone = TiltDeadZoneL) {
		return DeadZoneLInfluence(LStick_X, DeadZone);
	}
	/// <summary>
	/// ���X�e�B�b�N�̌X��Y���擾����
	/// </summary>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneL</param>
	/// <returns></returns>
	static SHORT GetLStickTiltY(int DeadZone = TiltDeadZoneL) {
		return DeadZoneLInfluence(LStick_Y, DeadZone);
	}
	/// <summary>
	/// �������Ɏw�肵�������ɍ��X�e�B�b�N���X���Ă��邩���擾
	/// </summary>
	/// <param name="Direction">����</param>
	///<param name="   ">STICK_UP</param>
	///<param name="   ">STICK_DOWN</param>
	///<param name="   ">STICK_LEFT</param>
	///<param name="   ">STICK_RIGHT</param>
	///<param name="   ">STICK_UP_LEFT</param>
	///<param name="   ">STICK_UP_RIGHT</param>
	///<param name="   ">STICK_DOWN_LEFT</param>
	///<param name="   ">STICK_DOWN_RIGHT</param>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneL</param>
	/// <returns></returns>
	static bool GetLStickTiltDirection(StickTiltDirection Direction, int DeadZone = TiltDeadZoneL) {
		SHORT X, Y;
		X = DeadZoneLInfluence(LStick_X, DeadZone);
		Y = DeadZoneLInfluence(LStick_Y, DeadZone);
		switch (Direction) {
		case STICK_UP:
			if (Y > 0) return true;
			break;
		case STICK_DOWN:
			if (Y < 0) return true;
			break;
		case STICK_LEFT:
			if (X < 0) return true;
			break;
		case STICK_RIGHT:
			if (X > 0) return true;
			break;
		case STICK_UP_LEFT:
			if (X < 0 && Y > 0) return true;
			break;
		case STICK_UP_RIGHT:
			if (X > 0 && Y > 0) return true;
			break;
		case STICK_DOWN_LEFT:
			if (X < 0 && Y < 0) return true;
			break;
		case STICK_DOWN_RIGHT:
			if (X > 0 && Y < 0) return true;
			break;
		default:
			return false;
			break;
		}
		return false;
	}

	/// <summary>
	/// ���X�e�B�b�N�̌X���̊p�x���擾(�������0�x)
	/// </summary>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneL</param>
	/// <returns></returns>
	static double GetLStickTilt(int DeadZone = TiltDeadZoneL) {
		SHORT X, Y;
		if (DeadZoneLInfluence(LStick_X, DeadZone) == 0 &&
			DeadZoneLInfluence(LStick_Y, DeadZone) == 0) {
			X = 0;
			Y = 0;
		}
		else {
			X = LStick_X;
			Y = LStick_Y;
		}
		return atan2ToDegrees(X, Y);
	}
#pragma endregion
#pragma region RSticks
	/// <summary>
	/// �E�X�e�B�b�N�̃f�b�h�]�[����ύX����
	/// </summary>
	/// <param name="DeadZone">�ύX����l : MAX 32767 : MIN 0</param>
	static void ChangeDeadZoneR(int DeadZone) {
		if (DeadZone > 32767)DeadZone = 32767;
		if (DeadZone < 0)DeadZone = 0;
		TiltDeadZoneR = DeadZone;
	}
	//�f�b�h�]�[���ȉ��Ȃ�0��Ԃ��A�����łȂ���΂��̂܂ܕԂ�
	static SHORT DeadZoneRInfluence(SHORT valuem, int DeadZone = TiltDeadZoneR) {
		if (valuem <  TiltDeadZoneR &&
			valuem > -TiltDeadZoneR) {
			return 0;
		}
		return valuem;
	}
	//�f�b�h�]�[�����Ȃ�false��Ԃ��A�����łȂ����true��Ԃ�
	static bool isInDeadZoneR() {
		if (RStick_X <  TiltDeadZoneR &&
			RStick_X > -TiltDeadZoneR && 
			RStick_Y <  TiltDeadZoneR &&
			RStick_Y > -TiltDeadZoneR) {
			return false;
		}
		return true;
	}
	/// <summary>
	/// �E�X�e�B�b�N�̌X��X���擾����
	/// </summary>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneR</param>
	/// <returns></returns>
	static SHORT GetRStickTiltX(int DeadZone = TiltDeadZoneR) {
		return DeadZoneRInfluence(RStick_X, DeadZone);
	}
	/// <summary>
	/// �E�X�e�B�b�N�̌X��Y���擾����
	/// </summary>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneR</param>
	/// <returns></returns>
	static SHORT GetRStickTiltY(int DeadZone = TiltDeadZoneR) {
		return DeadZoneRInfluence(RStick_Y, DeadZone);
	}
	/// <summary>
	/// �������Ɏw�肵�������ɉE�X�e�B�b�N���X���Ă��邩���擾
	/// </summary>
	/// <param name="Direction">����</param>
	///<param name="   ">STICK_UP</param>
	///<param name="   ">STICK_DOWN</param>	
	///<param name="   ">STICK_LEFT</param>
	///<param name="   ">STICK_RIGHT</param>
	///<param name="   ">STICK_UP_LEFT</param>
	///<param name="   ">STICK_UP_RIGHT</param>
	///<param name="   ">STICK_DOWN_LEFT</param>
	///<param name="   ">STICK_DOWN_RIGHT</param>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneR</param>
	/// <returns></returns>
	static bool GetRStickTiltDirection(StickTiltDirection Direction, int DeadZone = TiltDeadZoneR) {
		SHORT X, Y;
		X = DeadZoneRInfluence(RStick_X, DeadZone);
		Y = DeadZoneRInfluence(RStick_Y, DeadZone);
		switch (Direction) {
		case STICK_UP:
			if (Y > 0) return true;
			break;
		case STICK_DOWN:
			if (Y < 0) return true;
			break;
		case STICK_LEFT:
			if (X < 0) return true;
			break;
		case STICK_RIGHT:
			if (X > 0) return true;
			break;
		case STICK_UP_LEFT:
			if (X < 0 && Y > 0) return true;
			break;
		case STICK_UP_RIGHT:
			if (X > 0 && Y > 0) return true;
			break;
		case STICK_DOWN_LEFT:
			if (X < 0 && Y < 0) return true;
			break;
		case STICK_DOWN_RIGHT:
			if (X > 0 && Y < 0) return true;
			break;
		default:
			return false;
			break;
		}
		return false;
	}

	/// <summary>
	/// �E�X�e�B�b�N�̌X���̊p�x���擾(�������0�x)
	/// </summary>
	/// <param name="DeadZone">���͂����m���Ȃ��͈� �f�t�H���g�� TiltDeadZoneR</param>
	/// <returns></returns>
	static double GetRStickTilt(int DeadZone = TiltDeadZoneR) {
		SHORT X, Y;
		if (DeadZoneRInfluence(RStick_X, DeadZone) == 0 &&
			DeadZoneRInfluence(RStick_Y, DeadZone) == 0) {
			X = 0;
			Y = 0;
		}
		else {
			X = RStick_X;
			Y = RStick_Y;
		}
		return atan2ToDegrees(X, Y);
	}
	//--------------------------------//
#pragma endregion
	static void Vibration(WORD left, WORD right, float Flame) {
		if (vibration.wLeftMotorSpeed < left) {
			if (left > 65535) {
				left = 65535;
			}
			vibration.wLeftMotorSpeed = left;
		}
		if (vibration.wRightMotorSpeed < right) {
			if (right > 65535) {
				right = 65535;
			}
			vibration.wRightMotorSpeed = right;
		}
		if (vibrationFlame < Flame) {
			vibrationFlame = int(Flame);
		}
	}
#pragma endregion
};

static constexpr bool operator== (Input::MouseMove &v1, Input::MouseMove &v2)
{
	return ((v1.IX == v2.IX) && (v1.IY == v2.IY) && (v1.IZ == v2.IZ));
}

static constexpr bool operator!= (Input::MouseMove &v1, Input::MouseMove &v2)
{
	return !((v1.IX == v2.IX) && (v1.IY == v2.IY) && (v1.IZ == v2.IZ));
}