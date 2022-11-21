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

#pragma region API_Input関連

//-------------マウス関連-------------//
#define API_MOUSE_LEFT          0x01     //マウス左ボタン
#define API_MOUSE_RIGHT         0x02     //マウス右ボタン
#define API_MOUSE_CENTER        0x04     //マウス中央ボタン
#define API_MOUSE_BUTTON_1      0x05     //マウス第1拡張ボタン
#define API_MOUSE_BUTTON_2      0x06     //マウス第2拡張ボタン



//-------------テンキー関連-------------//
//---0～9---///
#define API_KEY_NUMPAD_0        0x60     //0キー
#define API_KEY_NUMPAD_1        0x23     //1キー
#define API_KEY_NUMPAD_2        0x28     //2キー
#define API_KEY_NUMPAD_3        0x22     //3キー
#define API_KEY_NUMPAD_4        0x25     //4キー
#define API_KEY_NUMPAD_5        0x0C     //5キー
#define API_KEY_NUMPAD_6        0x27     //6キー
#define API_KEY_NUMPAD_7        0x24     //7キー
#define API_KEY_NUMPAD_8        0x26     //8キー
#define API_KEY_NUMPAD_9        0x21     //9キー
//---NUMLOCK 0～9---///
#define API_KEY_NUMPAD_L_0      0x60     //NumLock && 0キー
#define API_KEY_NUMPAD_L_1      0x61     //NumLock && 1キー
#define API_KEY_NUMPAD_L_2      0x62     //NumLock && 2キー
#define API_KEY_NUMPAD_L_3      0x63     //NumLock && 3キー
#define API_KEY_NUMPAD_L_4      0x64     //NumLock && 4キー
#define API_KEY_NUMPAD_L_5      0x65     //NumLock && 5キー
#define API_KEY_NUMPAD_L_6      0x66     //NumLock && 6キー
#define API_KEY_NUMPAD_L_7      0x67     //NumLock && 7キー
#define API_KEY_NUMPAD_L_8      0x68     //NumLock && 8キー
#define API_KEY_NUMPAD_L_9      0x69     //NumLock && 9キー
//---  +  -  *  /  小数点  ---///
#define API_KEY_MULTI           0x6A     //テンキーの * キー
#define API_KEY_ADD             0x6B     //テンキーの + キー
#define API_KEY_SEPARATOR       0x6C     //テンキーの Enter キー
#define API_KEY_SUB             0x6D     //テンキーの - キー
#define API_KEY_DECIMAL         0x6E     //テンキーの . (小数点)キー
#define API_KEY_DIVIDE          0x6F     //テンキーの / キー
#define API_KEY_OFF_DECIMAL     0x2E     //NumLockオフのテンキー[.]



//-------------0～9-------------//
#define API_KEY_0               0x30     //0キー
#define API_KEY_1               0x31     //1キー
#define API_KEY_2               0x32     //2キー
#define API_KEY_3               0x33     //3キー
#define API_KEY_4               0x34     //4キー
#define API_KEY_5               0x35     //5キー
#define API_KEY_6               0x36     //6キー
#define API_KEY_7               0x37     //7キー
#define API_KEY_8               0x38     //8キー
#define API_KEY_9               0x39     //9キー
//-------------A～Zキー関連-------------//
#define API_KEY_A               0x41     //Aキー
#define API_KEY_B               0x42     //Bキー
#define API_KEY_C               0x43     //Cキー
#define API_KEY_D               0x44     //Dキー
#define API_KEY_E               0x45     //Eキー
#define API_KEY_F               0x46     //Fキー
#define API_KEY_G               0x47     //Gキー
#define API_KEY_H               0x48     //Hキー
#define API_KEY_I               0x49     //Iキー
#define API_KEY_J               0x4A     //Jキー
#define API_KEY_K               0x4B     //Kキー
#define API_KEY_L               0x4C     //Lキー
#define API_KEY_M               0x4D     //Mキー
#define API_KEY_N               0x4E     //Nキー
#define API_KEY_O               0x4F     //Oキー
#define API_KEY_P               0x50     //Pキー
#define API_KEY_Q               0x51     //Qキー
#define API_KEY_R               0x52     //Rキー
#define API_KEY_S               0x53     //Sキー
#define API_KEY_T               0x54     //Tキー
#define API_KEY_U               0x55     //Uキー
#define API_KEY_V               0x56     //Vキー
#define API_KEY_W               0x57     //Wキー
#define API_KEY_X               0x58     //Xキー
#define API_KEY_Y               0x59     //Yキー
#define API_KEY_Z               0x5A     //Zキー



//-------------矢印キー-------------//
#define API_KEY_LEFT            0x25     //Leftキー || NumLockオフのテンキー[4]
#define API_KEY_UP              0x26     //Upキー || NumLockオフのテンキー[8]
#define API_KEY_RIGHT           0x27     //Rightキー || NumLockオフのテンキー[6]
#define API_KEY_DOWN            0x28     //Downキー || NumLockオフのテンキー[2]



//-------------ファンクションキー-------------//
#define API_KEY_F1              0x70     //F1キー
#define API_KEY_F2              0x71     //F2キー
#define API_KEY_F3              0x72     //F3キー
#define API_KEY_F4              0x73     //F4キー
#define API_KEY_F5              0x74     //F5キー
#define API_KEY_F6              0x75     //F6キー
#define API_KEY_F7              0x76     //F7キー
#define API_KEY_F8              0x77     //F8キー
#define API_KEY_F9              0x78     //F9キー
#define API_KEY_F10             0x79     //F10キー
#define API_KEY_F11             0x7A     //F11キー
#define API_KEY_F12             0x7B     //F12キー
#define API_KEY_F13             0x7C     //F13キー
#define API_KEY_F14             0x7D     //F14キー
#define API_KEY_F15             0x7E     //F15キー
#define API_KEY_F16             0x7F     //F16キー
#define API_KEY_F17             0x80     //F17キー
#define API_KEY_F18             0x81     //F18キー
#define API_KEY_F19             0x82     //F19キー
#define API_KEY_F20             0x83     //F20キー
#define API_KEY_F21             0x84     //F21キー
#define API_KEY_F22             0x85     //F22キー
#define API_KEY_F23             0x86     //F23キー
#define API_KEY_F24             0x87     //F24キー



//-------------その他のキーボード-------------//
#define API_KEY_BACK            0x08     //Backspaceキー
#define API_KEY_TAB             0x09     //Tabキー
#define API_KEY_CLEAR           0x0C     //Clearキー || NumLockオフのテンキー[5]
#define API_KEY_ENTER           0x0D     //Enterキー || テンキーEnterキー
#define API_KEY_SHIFT           0x10     //Shiftキー
#define API_KEY_LSHIFT          0xA0     //左Shiftキー
#define API_KEY_RSHIFT          0xA1     //右Shiftキー
#define API_KEY_CONTROL         0x11     //Ctrlキー
#define API_KEY_LCONTROL        0xA2     //左Ctrlキー
#define API_KEY_RCONTROL        0xA3     //右Ctrlキー
#define API_KEY_MENU            0x12     //Altキー
#define API_KEY_LMENU           0xA4     //左Altキー
#define API_KEY_RMENU           0xA5     //右Altキー
#define API_KEY_PAUSE           0x13     //Pauseキー
#define API_KEY_ESCAPE          0x1B     //Escキー
#define API_KEY_SPACE           0x20     //Spaceキー
#define API_KEY_PGUP            0x21     //PageUpキー || NumLockオフのテンキー[9]
#define API_KEY_PGDN            0x22     //PageDownキー || NumLockオフのテンキー[3]
#define API_KEY_END             0x23     //Endキー || NumLockオフのテンキー[1]
#define API_KEY_HOME            0x24     //Homeキー || NumLockオフのテンキー[7]
#define API_KEY_SELECT          0x29     //Selectキー
#define API_KEY_PRINT           0x2A     //Printキー
#define API_KEY_EXECUTE_        0x2B     //Executeキー //再定義回避
#define API_KEY_SNAPSHOT        0x2C     //PrintScreenキー
#define API_KEY_INSERT          0x2D     //Insertキー || NumLockオフのテンキー[0]
#define API_KEY_DELETE          0x2E     //Deleteキー || NumLockオフのテンキー[.]
#define API_KEY_HELP            0x2F     //Helpキー
#define API_KEY_LWIN            0x5B     //左Windowsキー
#define API_KEY_RWIN            0x5C     //右Windowsキー
#define API_KEY_APPS            0x5D     //アプリケーションキー
#define API_KEY_SLEEP           0x5F     //コンピュータースリープキー
#define API_KEY_NUMLOCK         0x90     //NumLockキー
#define API_KEY_SCROLL          0x91     //ScrollLockキー
#define API_KEY_ATTN            0xF6     //Attnキー
#define API_KEY_CRSEL           0xF7     //CrSelキー
#define API_KEY_EXSEL           0xF8     //ExSelキー
#define API_KEY_EREOF           0xF9     //Erase EOFキー
#define API_KEY_PLAY            0xFA     //Playキー
#define API_KEY_Zoom            0xFB     //Zoomキー
#define API_KEY_PA1             0xFD     //PA1キー



//-------------INE関連-------------//
#define API_IME_KANA            0x15     //IME かな モード
#define API_IME_JUNJA           0x17     //IME Junja モード
#define API_IME_FINAL           0x18     //IME ファイナル モード
#define API_IME_KANJI           0x19     //IME 漢字 モード [Alt]+[半角／全角]
#define API_IME_CONVERT         0x1C     //IME 変換 [変換]
#define API_IME_NONCONVERT      0x1D     //IME 無変換 [無変換]
#define API_IME_ACCEPT          0x1E     //IME 使用可能
#define API_IME_MODECHANGE      0x1F     //IME モード変更要求
#define API_IME_PROCESS         0xE5     //IME PROCESSキー



//-------------OEM関連-------------//
#define API_OEM_NULL_1          0x92     //OEM固有 不明
#define API_OEM_NULL_2          0x93     //OEM固有 不明
#define API_OEM_NULL_3          0x94     //OEM固有 不明
#define API_OEM_NULL_4          0x95     //OEM固有 不明
#define API_OEM_NULL_5          0x96     //OEM固有 不明
#define API_OEM_MULTI           0xBA     // : * キー
#define API_OEM_COLON           0xBA     // : * キー
#define API_OEM_1               0xBA     // : * キー
#define API_OEM_PULS            0xBB     // ; + キー
#define API_OEM_SEMICOLON       0xBB     // ; + キー
#define API_OEM_CONMA           0xBC     // , < キー
#define API_OEM_SYOUNARI        0xBC     // , < キー
#define API_OEM_SUB             0xBD     // - = キー
#define API_OEM_SAME            0xBD     // - = キー
#define API_OEM_PERIOD          0xBE     // . > キー
#define API_OEM_DAINARI         0xBE     // . > キー
#define API_OEM_SLASH           0xBF     // / ? キー
#define API_OEM_QUESTION        0xBF     // / ? キー
#define API_OEM_2               0xBF     // / ? キー
#define API_OEM_ATMARK          0xC0     // @ ` キー
#define API_OEM_3               0xC0     // @ ` キー
#define API_OEM_4               0xDB     // [ { キー
#define API_OEM_5               0xDC     // \ | キー
#define API_OEM_6               0xDD     // ] } キー
#define API_OEM_7               0xDE     // ^ ~ キー
#define API_OEM_8               0xCF     //不明
#define API_OEM_AX              0xE1     //OEM固有 不明
#define API_OEM_102             0xE2     // バックスラッシュ _ キー
#define API_OEM_HELP            0xE3     //OEM固有 不明
#define API_OEM_00              0xE4     //OEM固有 不明
#define API_OEM_CLEAR           0xFE     //Clearキー
#define API_OEM_RESET           0xE9     //OEM固有 不明
#define API_OEM_JUMP            0xEA     //OEM固有 不明
#define API_OEM_PA1             0xEB     //OEM固有 不明
#define API_OEM_PA2             0xEC     //OEM固有 不明
#define API_OEM_PA3             0xED     //OEM固有 不明
#define API_OEM_WSCTRL          0xEE     //OEM固有 不明
#define API_OEM_CUSEL           0xEF     //OEM固有 不明
#define API_OEM_ATTN            0xF0     //OEM固有 [英数 CapsLock]
#define API_OEM_FINISH          0xF1     //OEM固有 不明
#define API_OEM_COPY            0xF2     //OEM固有 [カタカナ ひらがな]
#define API_OEM_AUTO            0xF3     //OEM固有 [半角／全角]
#define API_OEM_ENLW            0xF4     //OEM固有 不明
#define API_OEM_BACKTAB         0xF5     //OEM固有 [Alt]+[カタカナ ひらがな]



//-------------複合キー関連-------------//
#define API_KEY_CANCEL          0x03     //[Ctrl]+[Pause]  ||  [Ctrl]+[ScrollLock]
#define API_KEY_CAPITAL         0x14     //[Shift] && [英数 CapsLock]



//-------------ブラウザ関連-------------//
#define API_BROWSER_BACK        0xA6     //ブラウザの戻るキー
#define API_BROWSER_FORWARD     0xA7     //ブラウザの進むキー
#define API_BROWSER_REFRESH     0xA8     //ブラウザの更新キー
#define API_BROWSER_STOP        0xA9     //ブラウザの停止キー
#define API_BROWSER_SEARCH      0xAA     //ブラウザの検索キー
#define API_BROWSER_FAVORITES   0xAB     //ブラウザのお気に入りキー
#define API_BROWSER_HOME        0xAC     //ブラウザの開始およびホームキー



//-------------音声関連-------------//
#define API_VOLUME_MUTE         0xAD     //音量ミュートキー
#define API_VOLUME_DOWN         0xAE     //音量ダウンキー
#define API_VOLUME_UP           0xAF     //音量アップキー



//-------------メディア関連-------------//
#define API_MEDIA_NEXT_TRACK    0xB0     //次のトラックキー
#define API_MEDIA_PREV_TRACK    0xB1     //前のトラックキー
#define API_MEDIA_STOP          0xB2     //メディア停止キー
#define API_MEDIA_PLAY_PAUSE    0xB3     //メディア再生/一時停止キー



//-------------LAUNCH関連-------------//
#define API_LAUNCH_MAIL         0xB4     //メール起動キー
#define API_LAUNCH_MEDIA_SELECT 0xB5   //メディア選択キー
#define API_LAUNCH_APP1         0xB6     //アプリケーション1起動キー
#define API_LAUNCH_APP2         0xB7     //アプリケーション2起動キー



//-------------その他-------------//
#define API_OTHER_PACKET        0xE7     //キーボード以外の入力手段に使用される32 ビット仮想キー値の下位ワード	
//-------------他-------------//
/*
0x07		未定義
0x0A		予約済
0x0B		予約済
0x0E		未定義
0x0F		未定義
0x1A		未定義
0x3A		未定義
0x3B		未定義
0x3C		未定義
0x3D		未定義
0x3E		未定義
0x3F		未定義
0x40		未定義
0x88		割当無し
0x89		割当無し
0x8A		割当無し
0x8B		割当無し
0x8C		割当無し
0x8D		割当無し
0x8E		割当無し
0x8F		割当無し
0x97		割当無し
0x98		割当無し
0x99		割当無し
0x9A		割当無し
0x9B		割当無し
0x9C		割当無し
0x9D		割当無し
0x9E		割当無し
0x9F		割当無し
0xB8		予約済
0xB9		予約済
0xC1		予約済
0xC2		予約済
0xC3		予約済
0xC4		予約済
0xC5		予約済
0xC6		予約済
0xC7		予約済
0xC8		予約済
0xC9		予約済
0xCA		予約済
0xCB		予約済
0xCC		予約済
0xCD		予約済
0xCE		予約済
0xCF		予約済
0xD0		予約済
0xD1		予約済
0xD2		予約済
0xD3		予約済
0xD4		予約済
0xD5		予約済
0xD6		予約済
0xD7		予約済
0xD8		割当無し
0xD9		割当無し
0xDA		割当無し
0xE0		予約済
0xE8		割当無し
0xFC	VK_NONAME	予約済
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
			/* 現在のキー情報を取得 */
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
//マウス
enum MouseButton {
	M_LEFT,
	M_RIGHT,
	M_CENTER
};
//ゲームパッド
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
#define D_X_LTHUMB		XINPUT_GAMEPAD_LEFT_THUMB  //スティックの押し込み?
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
	// ゲームパッドデバイスの作成
	static bool CreateGamePadDevice();
	static bool StartGamePadControl();
	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static bool SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device);
	static bool SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device);
	//ゲームパッドの更新
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

	//Inputの初期化
	static void Initialize(Window *win);

	//----------入力状態の更新----------//

	static void Update();

	//----------各入力状態取得関数系----------//
#pragma region DirectInput入力状態
#pragma region キーボード
	//キーが押されているか
	static bool isKey(int keyIndex) {
		if (keyIndex < 0 || keyIndex > 255)return false;
		return keys[keyIndex];
	}
	//キーが押された瞬間か
	static bool isKeyTrigger(int keyIndex){
		if (keyIndex < 0 || keyIndex > 255)return false;
		return keys[keyIndex] && !oldkeys[keyIndex];
	}
	//キーを離した瞬間か
	static bool isKeyRelease(int keyIndex) {
		if (keyIndex < 0 || keyIndex > 255)return false;
		return !keys[keyIndex] && oldkeys[keyIndex];
	}
#pragma endregion
#pragma region マウス
	//マウスのボタンが押されているか
	static bool isMouse(int mouseIndex) {
		if (mouseIndex < 0 || mouseIndex > 255)return false;
		return mouse.rgbButtons[mouseIndex];
	}
	//マウスのボタンが押された瞬間か
	static bool isMouseTrigger(int mouseIndex) {
		if (mouseIndex < 0 || mouseIndex > 255)return false;
		return mouse.rgbButtons[mouseIndex] && !oldmouse.rgbButtons[mouseIndex];
	}
	//マウスのボタンを離した瞬間か
	static bool isMouseRelease(int mouseIndex) {
		if (mouseIndex < 0 || mouseIndex > 255)return false;
		return !mouse.rgbButtons[mouseIndex] && oldmouse.rgbButtons[mouseIndex];
	}
	//マウスの座標を取得
	static XMFLOAT2 GetMousePos() {
		return mousePos;
	}
	//マウスのレイを取得
	static Ray GetMouseRay();
	//マウスの移動量を取得
	static MouseMove GetMouseMove() {
		MouseMove tmp;
		tmp.IX = mouse.lX;
		tmp.IY = mouse.lY;
		tmp.IZ = mouse.lZ;
		return tmp;
	}
	//ホイール
	static LONG GetMouseWheelMove() {
		LONG IZ = mouse.lZ;
		return IZ;
	}
#pragma endregion
#pragma region コントローラー
	//コントローラーのボタンが押されているか
	static bool DirectIsButton(ButtonKind button) {
		if (g_ButtonStates[button] == ButtonState::ButtonStatePush) return true;
		return false;
	}
	//コントローラーのボタンが押された瞬間か
	static bool DirectIsButtonTrigger(ButtonKind button) {
		if (g_ButtonStates[button] == ButtonState::ButtonStateUp) return true;
		return false;
	}
	//コントローラーのボタンを離した瞬間か
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
	//コントローラーが認識されたかどうか
	static bool isConnectedXGamePad;
	//XInput構造体
	static XINPUT_STATE XGamePad;
	// 振動
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
	static int TiltDeadZoneL; //左スティックのデッドゾーン
	static int TiltDeadZoneR; //右スティックのデッドゾーン
	//----------各入力状態取得関数系----------//
#pragma region Buttons
	//コントローラーのボタンが押されているか
	static bool IsButton(XInputButtonState button) {
		return PadButton[button];
	}
	//コントローラーのボタンが押された瞬間か
	static bool IsButtonTrigger(XInputButtonState button) {
		return PadButton[button] && !OldPadButton[button];
	}
	//コントローラーのボタンを離した瞬間か
	static bool IsButtonRelease(XInputButtonState button) {
		return !PadButton[button] && OldPadButton[button];
	}
#pragma endregion
#pragma region LSticks
	/// <summary>
	/// 左スティックのデッドゾーンを変更する
	/// </summary>
	/// <param name="DeadZone">変更する値 : MAX 32767 : MIN 0</param>
	static void ChangeDeadZoneL(int DeadZone) {
		if (DeadZone > 32767)DeadZone = 32767;
		if (DeadZone < 0)DeadZone = 0;
		TiltDeadZoneL = DeadZone;
	}
	//デッドゾーン以下なら0を返す、そうでなければそのまま返す
	static SHORT DeadZoneLInfluence(SHORT valuem, int DeadZone = TiltDeadZoneL) {
		if (valuem <  TiltDeadZoneL &&
			valuem > -TiltDeadZoneL) {
			return 0;
		}
		return valuem;
	}
	//デッドゾーン内ならfalseを返す、そうでなければtrueを返す
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
	/// 左スティックの傾きXを取得する
	/// </summary>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneL</param>
	/// <returns></returns>
	static SHORT GetLStickTiltX(int DeadZone = TiltDeadZoneL) {
		return DeadZoneLInfluence(LStick_X, DeadZone);
	}
	/// <summary>
	/// 左スティックの傾きYを取得する
	/// </summary>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneL</param>
	/// <returns></returns>
	static SHORT GetLStickTiltY(int DeadZone = TiltDeadZoneL) {
		return DeadZoneLInfluence(LStick_Y, DeadZone);
	}
	/// <summary>
	/// 第一引数に指定した方向に左スティックが傾いているかを取得
	/// </summary>
	/// <param name="Direction">方向</param>
	///<param name="   ">STICK_UP</param>
	///<param name="   ">STICK_DOWN</param>
	///<param name="   ">STICK_LEFT</param>
	///<param name="   ">STICK_RIGHT</param>
	///<param name="   ">STICK_UP_LEFT</param>
	///<param name="   ">STICK_UP_RIGHT</param>
	///<param name="   ">STICK_DOWN_LEFT</param>
	///<param name="   ">STICK_DOWN_RIGHT</param>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneL</param>
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
	/// 左スティックの傾きの角度を取得(上方向が0度)
	/// </summary>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneL</param>
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
	/// 右スティックのデッドゾーンを変更する
	/// </summary>
	/// <param name="DeadZone">変更する値 : MAX 32767 : MIN 0</param>
	static void ChangeDeadZoneR(int DeadZone) {
		if (DeadZone > 32767)DeadZone = 32767;
		if (DeadZone < 0)DeadZone = 0;
		TiltDeadZoneR = DeadZone;
	}
	//デッドゾーン以下なら0を返す、そうでなければそのまま返す
	static SHORT DeadZoneRInfluence(SHORT valuem, int DeadZone = TiltDeadZoneR) {
		if (valuem <  TiltDeadZoneR &&
			valuem > -TiltDeadZoneR) {
			return 0;
		}
		return valuem;
	}
	//デッドゾーン内ならfalseを返す、そうでなければtrueを返す
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
	/// 右スティックの傾きXを取得する
	/// </summary>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneR</param>
	/// <returns></returns>
	static SHORT GetRStickTiltX(int DeadZone = TiltDeadZoneR) {
		return DeadZoneRInfluence(RStick_X, DeadZone);
	}
	/// <summary>
	/// 右スティックの傾きYを取得する
	/// </summary>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneR</param>
	/// <returns></returns>
	static SHORT GetRStickTiltY(int DeadZone = TiltDeadZoneR) {
		return DeadZoneRInfluence(RStick_Y, DeadZone);
	}
	/// <summary>
	/// 第一引数に指定した方向に右スティックが傾いているかを取得
	/// </summary>
	/// <param name="Direction">方向</param>
	///<param name="   ">STICK_UP</param>
	///<param name="   ">STICK_DOWN</param>	
	///<param name="   ">STICK_LEFT</param>
	///<param name="   ">STICK_RIGHT</param>
	///<param name="   ">STICK_UP_LEFT</param>
	///<param name="   ">STICK_UP_RIGHT</param>
	///<param name="   ">STICK_DOWN_LEFT</param>
	///<param name="   ">STICK_DOWN_RIGHT</param>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneR</param>
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
	/// 右スティックの傾きの角度を取得(上方向が0度)
	/// </summary>
	/// <param name="DeadZone">入力を感知しない範囲 デフォルトは TiltDeadZoneR</param>
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