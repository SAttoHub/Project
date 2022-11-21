#include"input.h"
#include <tchar.h>

SHORT WinAPIInput::keys[256] = { 0 };
SHORT WinAPIInput::oldkeys[256] = { 0 };

IDirectInput8 *Input::dinput;
IDirectInputDevice8 *Input::devkeyborad;
IDirectInputDevice8 *Input::devmouse;
LPDIRECTINPUTDEVICE8 Input::devGamePad;

BYTE Input::keys[256];
BYTE Input::oldkeys[256];

DIMOUSESTATE2 Input::mouse = { 0 };
DIMOUSESTATE2 Input::oldmouse = { 0 };
XMFLOAT2 Input::mousePos = { 0,0 };
Ray Input::mouseRay;
ButtonState Input::g_ButtonStates[ButtonKind::ButtonKindMax];

bool Input::isConnectGamePad;


//-------------XInput-------------//
XINPUT_VIBRATION Input::vibration = {};
int Input::vibrationFlame = 0;
bool Input::PadButton[MAX_BUTTONS] = { 0 };
bool Input::OldPadButton[MAX_BUTTONS] = { 0 };
SHORT Input::LStick_X;
SHORT Input::LStick_Y;
SHORT Input::RStick_X;
SHORT Input::RStick_Y;
int Input::TiltDeadZoneL = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
int Input::TiltDeadZoneR = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
XINPUT_STATE Input::XGamePad;
bool Input::isConnectedXGamePad;
//--------------------------------//

bool Input::CreateGamePadDevice() {
    EnumParameter parameter;
    parameter.FindCount = 0;
    parameter._devGamePad = &devGamePad;

    // GAMEPADを調べる
    dinput->EnumDevices(
        DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類
        DeviceFindCallBack,			// 発見時に実行する関数
        &parameter,					// 関数に渡す値
        DIEDFL_ATTACHEDONLY			// 検索方法
    );
    // JOYSTICKを調べる
    dinput->EnumDevices(
        DI8DEVTYPE_JOYSTICK,
        DeviceFindCallBack,
        &parameter,
        DIEDFL_ATTACHEDONLY
    );
    // どちらも見つけることが出来なかったら失敗
    if (parameter.FindCount == 0) {
        return false;
    }
    int count = 0;
    // 制御開始
    while (StartGamePadControl() == false) {
        Sleep(100);
        count++;
        if (count >= 5) {
            break;
        }
    }
    return true;
}

bool Input::StartGamePadControl() {
    if (devGamePad == nullptr) {
        return false;
    }
    // 制御開始
    if (FAILED(devGamePad->Acquire())){
        return false;
    }
    DIDEVCAPS cap;
    devGamePad->GetCapabilities(&cap);
    // ポーリング判定
    if (cap.dwFlags & DIDC_POLLEDDATAFORMAT) {
        DWORD error = GetLastError();
        // ポーリング開始
        if (FAILED(devGamePad->Poll())) {
            return false;
        }
    }
    return true;
}

BOOL CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
    EnumParameter *parameter = (EnumParameter *)pvRef;
    LPDIRECTINPUTDEVICE8 device = nullptr;

    // 既に発見しているなら終了
    if (parameter->FindCount >= 1)
    {
        return DIENUM_STOP;
    }

    // デバイス生成
    HRESULT hr = dinput->CreateDevice(
        lpddi->guidInstance,
        parameter->_devGamePad,
        NULL);

    if (FAILED(hr))
    {
        return DIENUM_STOP;
    }

    // 入力フォーマットの指定
    device = *parameter->_devGamePad;
    hr = device->SetDataFormat(&c_dfDIJoystick);

    if (FAILED(hr))
    {
        return DIENUM_STOP;
    }

    // プロパティの設定
    if (SetUpGamePadProperty(device) == false)
    {
        return DIENUM_STOP;
    }

    // 協調レベルの設定
    if (SetUpCooperativeLevel(device) == false)
    {
        return DIENUM_STOP;
    }

    // 発見数をカウント
    parameter->FindCount++;

    return DIENUM_CONTINUE;
}

void Input::Initialize(Window *win) {
    HRESULT result;
    result = DirectInput8Create(
        win->WindowData.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr
    );
    if (FAILED(result)) {
        exit(1);
    }
    result = dinput->CreateDevice(
        GUID_SysKeyboard, &devkeyborad, NULL
    );
    result = devkeyborad->SetDataFormat(&c_dfDIKeyboard);
    result = devkeyborad->SetCooperativeLevel(
        win->WinHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
    );
    //マウス
    result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
    result = devmouse->SetDataFormat(&c_dfDIMouse);
    result = devmouse->SetCooperativeLevel(
        win->WinHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    //ゲームパッド
    isConnectGamePad = CreateGamePadDevice();
    //GamePadが接続されている場合のみ
    if (isConnectGamePad) {
        // 入力情報の初期化
        for (int i = 0; i < ButtonKind::ButtonKindMax; i++) {
            g_ButtonStates[i] = ButtonState::ButtonStateNone;
        }
    }
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
}

void Input::Update() {
    for (int i = 0; i < 256; i++) {
        oldkeys[i] = keys[i];
    }
    devkeyborad->Acquire();
    devkeyborad->GetDeviceState(sizeof(keys), keys);

    //マウス
    oldmouse = mouse;
    devmouse->Acquire();
    devmouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(FindWindowA("Window", nullptr), &p);
    mousePos.x = (float)p.x;
    mousePos.y = (float)p.y;
    //GamePadが接続されている場合のみ
    if (isConnectGamePad) {
        UpdateGamePad();
    }
    //XInout更新
    UpdateXInput();
}

bool Input::SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device) {
    // 軸モードを絶対値モードとして設定
    DIPROPDWORD diprop;
    ZeroMemory(&diprop, sizeof(diprop));
    diprop.diph.dwSize = sizeof(diprop);
    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.diph.dwObj = 0;
    diprop.dwData = DIPROPAXISMODE_ABS;
    if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
        return false;
    }
    // X軸の値の範囲設定
    DIPROPRANGE diprg;
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.diph.dwObj = DIJOFS_X;
    diprg.lMin = -1000;
    diprg.lMax = 1000;
    if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) {
        return false;
    }
    // Y軸の値の範囲設定
    diprg.diph.dwObj = DIJOFS_Y;
    if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) {
        return false;
    }

    return true;
}

bool Input::SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device) {
    //協調モードの設定
    if (FAILED(device->SetCooperativeLevel(
        FindWindow(_T("Window"), nullptr),
        DISCL_EXCLUSIVE | DISCL_FOREGROUND)
    )) {
        return false;
    }

    return true;
}

void Input::UpdateGamePad() {
    if (devGamePad == nullptr){
        return;
    }

    DIJOYSTATE pad_data;

    // デバイス取得
    HRESULT hr = devGamePad->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
    if (FAILED(hr))
    {
        // 再度制御開始
        if (FAILED(devGamePad->Acquire()))
        {
            for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
            {
                g_ButtonStates[i] = ButtonState::ButtonStateNone;
            }
            devGamePad->Poll();
        }
        return;
    }

    bool is_push[ButtonKind::ButtonKindMax] = {false};
    // スティック判定
    int unresponsive_range = 200;
    if (pad_data.lX < -unresponsive_range)
    {
        is_push[ButtonKind::LeftButton] = true;
    }
    else if (pad_data.lX > unresponsive_range)
    {
        is_push[ButtonKind::RightButton] = true;
    }

    if (pad_data.lY < -unresponsive_range)
    {
        is_push[ButtonKind::UpButton] = true;
    }
    else if (pad_data.lY > unresponsive_range)
    {
        is_push[ButtonKind::DownButton] = true;
    }

    // 十字キー判定
    if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
    {
        float PI = 3.141592654f;
        float rad = ((pad_data.rgdwPOV[0] / 100.0f) * (PI / 180.0f));
        // 本来はxがcos、yがsinだけど、rgdwPOVは0が上から始まるので、
        // cosとsinを逆にした方が都合がいい
        float x = sinf(rad);
        float y = cosf(rad);

        if (x < -0.01f)
        {
            is_push[ButtonKind::LeftButton] = true;
        }
        else if (x > 0.01f)
        {
            is_push[ButtonKind::RightButton] = true;
        }

        if (y > 0.01f)
        {
            is_push[ButtonKind::UpButton] = true;
        }
        else if (y < -0.01f)
        {
            is_push[ButtonKind::DownButton] = true;
        }
    }

    // ボタン判定
    for (int i = 0; i < 32; i++)
    {
        if (!(pad_data.rgbButtons[i] & 0x80))
        {
            continue;
        }

        switch (i)
        {
        case 0:
            is_push[ButtonKind::ButtonA] = true;
            break;
        case 1:
            is_push[ButtonKind::ButtonB] = true;
            break;
        case 2:
            is_push[ButtonKind::ButtonX] = true;
            break;
        case 3:
            is_push[ButtonKind::ButtonY] = true;
            break;
        default:
            break;
        }
    }

    // 入力情報からボタンの状態を更新する
    for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
    {
        if (is_push[i] == true)
        {
            if (g_ButtonStates[i] == ButtonState::ButtonStateNone)
            {
                g_ButtonStates[i] = ButtonState::ButtonStateDown;
            }
            else
            {
                g_ButtonStates[i] = ButtonState::ButtonStatePush;
            }
        }
        else
        {
            if (g_ButtonStates[i] == ButtonState::ButtonStatePush)
            {
                g_ButtonStates[i] = ButtonState::ButtonStateUp;
            }
            else
            {
                g_ButtonStates[i] = ButtonState::ButtonStateNone;
            }
        }
    }
}

Ray Input::GetMouseRay() {
    XMFLOAT3 start = Camera::ConvertScreenToWorld(XMFLOAT2(mousePos.x - 5.0f, mousePos.y), 0.0f);
    mouseRay.start = { start.x,start.y,start.z,1 };
    XMFLOAT3 to = Camera::ConvertScreenToWorld(mousePos, 1.0f);
    XMFLOAT3 vec(to.x - start.x, to.y - start.y, to.z - start.z);
    mouseRay.dir = XMLoadFloat3(&vec);
    mouseRay.dir = XMVector3Normalize(mouseRay.dir);
    return mouseRay;
}

void Input::UpdateXInput() {
    ZeroMemory(&XGamePad, sizeof(XINPUT_STATE));
    DWORD dwResult = XInputGetState(0, &XGamePad);
    if (dwResult == ERROR_SUCCESS) {
        isConnectedXGamePad = true;
    }
    else {
        isConnectedXGamePad = false;
    }

    //入力配列の更新
    //全状態のリセット
    for (int i = 0; i < MAX_BUTTONS; i++) {
        OldPadButton[i] = PadButton[i]; //1フレーム前の入力状態の更新
        PadButton[i] = false;
        LStick_X = 0;
        LStick_Y = 0;
        RStick_X = 0;
        RStick_Y = 0;
    }
    if (isConnectedXGamePad) {
#pragma region wButtons
        if (XGamePad.Gamepad.wButtons & D_X_UP) PadButton[X_UP] = true;
        if (XGamePad.Gamepad.wButtons & D_X_DOWN) PadButton[X_DOWN] = true;
        if (XGamePad.Gamepad.wButtons & D_X_LEFT) PadButton[X_LEFT] = true;
        if (XGamePad.Gamepad.wButtons & D_X_RIGHT) PadButton[X_RIGHT] = true;
        if (XGamePad.Gamepad.wButtons & D_X_START) PadButton[X_START] = true;
        if (XGamePad.Gamepad.wButtons & D_X_BACK) PadButton[X_BACK] = true;
        if (XGamePad.Gamepad.wButtons & D_X_LTHUMB) PadButton[X_LTHUMB] = true;
        if (XGamePad.Gamepad.wButtons & D_X_RTHUMB) PadButton[X_RTHUMB] = true;
        if (XGamePad.Gamepad.wButtons & D_X_LB) PadButton[X_LB] = true;
        if (XGamePad.Gamepad.wButtons & D_X_RB) PadButton[X_RB] = true;
        if (XGamePad.Gamepad.wButtons & D_X_A) PadButton[X_A] = true;
        if (XGamePad.Gamepad.wButtons & D_X_B) PadButton[X_B] = true;
        if (XGamePad.Gamepad.wButtons & D_X_X) PadButton[X_X] = true;
        if (XGamePad.Gamepad.wButtons & D_X_Y) PadButton[X_Y] = true;
        //トリガーはボタンとは違い多段階のアナログ値なので入力の有無は閾値を超えたかどうかで判定する。
        if (XGamePad.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            PadButton[X_LT] = true;
        }
        if (XGamePad.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            PadButton[X_LT] = true;
        }
        //Lスティック
        LStick_X = XGamePad.Gamepad.sThumbLX;
        LStick_Y = XGamePad.Gamepad.sThumbLY;
        RStick_X = XGamePad.Gamepad.sThumbRX;
        RStick_Y = XGamePad.Gamepad.sThumbRY;
    }
#pragma endregion

    XInputSetState(0, &vibration);
    if (vibrationFlame == 0) {
        ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    }
    if (vibrationFlame > 0) {
        vibrationFlame--;
    }
}
