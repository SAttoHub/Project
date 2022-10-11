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

    // GAMEPAD�𒲂ׂ�
    dinput->EnumDevices(
        DI8DEVTYPE_GAMEPAD,			// ��������f�o�C�X�̎��
        DeviceFindCallBack,			// �������Ɏ��s����֐�
        &parameter,					// �֐��ɓn���l
        DIEDFL_ATTACHEDONLY			// �������@
    );
    // JOYSTICK�𒲂ׂ�
    dinput->EnumDevices(
        DI8DEVTYPE_JOYSTICK,
        DeviceFindCallBack,
        &parameter,
        DIEDFL_ATTACHEDONLY
    );
    // �ǂ���������邱�Ƃ��o���Ȃ������玸�s
    if (parameter.FindCount == 0) {
        return false;
    }
    int count = 0;
    // ����J�n
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
    // ����J�n
    if (FAILED(devGamePad->Acquire())){
        return false;
    }
    DIDEVCAPS cap;
    devGamePad->GetCapabilities(&cap);
    // �|�[�����O����
    if (cap.dwFlags & DIDC_POLLEDDATAFORMAT) {
        DWORD error = GetLastError();
        // �|�[�����O�J�n
        if (FAILED(devGamePad->Poll())) {
            return false;
        }
    }
    return true;
}

BOOL CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
    EnumParameter *parameter = (EnumParameter *)pvRef;
    LPDIRECTINPUTDEVICE8 device = nullptr;

    // ���ɔ������Ă���Ȃ�I��
    if (parameter->FindCount >= 1)
    {
        return DIENUM_STOP;
    }

    // �f�o�C�X����
    HRESULT hr = dinput->CreateDevice(
        lpddi->guidInstance,
        parameter->_devGamePad,
        NULL);

    if (FAILED(hr))
    {
        return DIENUM_STOP;
    }

    // ���̓t�H�[�}�b�g�̎w��
    device = *parameter->_devGamePad;
    hr = device->SetDataFormat(&c_dfDIJoystick);

    if (FAILED(hr))
    {
        return DIENUM_STOP;
    }

    // �v���p�e�B�̐ݒ�
    if (SetUpGamePadProperty(device) == false)
    {
        return DIENUM_STOP;
    }

    // �������x���̐ݒ�
    if (SetUpCooperativeLevel(device) == false)
    {
        return DIENUM_STOP;
    }

    // ���������J�E���g
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
    //�}�E�X
    result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
    result = devmouse->SetDataFormat(&c_dfDIMouse);
    result = devmouse->SetCooperativeLevel(
        win->WinHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    //�Q�[���p�b�h
    isConnectGamePad = CreateGamePadDevice();
    //GamePad���ڑ�����Ă���ꍇ�̂�
    if (isConnectGamePad) {
        // ���͏��̏�����
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

    //�}�E�X
    oldmouse = mouse;
    devmouse->Acquire();
    devmouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(FindWindowA("Window", nullptr), &p);
    mousePos.x = (float)p.x;
    mousePos.y = (float)p.y;
    //GamePad���ڑ�����Ă���ꍇ�̂�
    if (isConnectGamePad) {
        UpdateGamePad();
    }
    //XInout�X�V
    UpdateXInput();
}

bool Input::SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device) {
    // �����[�h���Βl���[�h�Ƃ��Đݒ�
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
    // X���̒l�͈̔͐ݒ�
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
    // Y���̒l�͈̔͐ݒ�
    diprg.diph.dwObj = DIJOFS_Y;
    if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) {
        return false;
    }

    return true;
}

bool Input::SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device) {
    //�������[�h�̐ݒ�
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

    // �f�o�C�X�擾
    HRESULT hr = devGamePad->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
    if (FAILED(hr))
    {
        // �ēx����J�n
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
    // �X�e�B�b�N����
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

    // �\���L�[����
    if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
    {
        float PI = 3.141592654f;
        float rad = ((pad_data.rgdwPOV[0] / 100.0f) * (PI / 180.0f));
        // �{����x��cos�Ay��sin�����ǁArgdwPOV��0���ォ��n�܂�̂ŁA
        // cos��sin���t�ɂ��������s��������
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

    // �{�^������
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

    // ���͏�񂩂�{�^���̏�Ԃ��X�V����
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

    //���͔z��̍X�V
    //�S��Ԃ̃��Z�b�g
    for (int i = 0; i < MAX_BUTTONS; i++) {
        OldPadButton[i] = PadButton[i]; //1�t���[���O�̓��͏�Ԃ̍X�V
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
        //�g���K�[�̓{�^���Ƃ͈Ⴂ���i�K�̃A�i���O�l�Ȃ̂œ��̗͂L����臒l�𒴂������ǂ����Ŕ��肷��B
        if (XGamePad.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            PadButton[X_LT] = true;
        }
        if (XGamePad.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            PadButton[X_LT] = true;
        }
        //L�X�e�B�b�N
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
