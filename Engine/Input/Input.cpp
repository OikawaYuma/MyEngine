#include "Input.h"
#include "WinAPI.h"
//Xinput.lib; Xinput9_1_0.lib
#include <iostream>
const float Input::DEADZONE_THRESHOLD = 0.1f;

void Input::Initialize() {
	WinAPI *sWinAPI = WinAPI::GetInstance();
	// DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		sWinAPI->GetWc().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	
	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		sWinAPI->GetHwnd(),DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
		}
		else
		{
			// Controller is not connected
		}
	}

	ZeroMemory(&preJoyState, sizeof(XINPUT_STATE));
	ZeroMemory(&joyState, sizeof(XINPUT_STATE));

}

void Input::Update() {
	// 前回のキー入力を保存
	memcpy(preKeys, keys, sizeof(keys));
	// 前回のキー入力を保存
	memcpy(&preJoyState, &joyState, sizeof(joyState));

	// キーボード情報の取得開始
	keyboard->Acquire();

	keyboard->GetDeviceState(sizeof(keys), keys);
}

bool Input::PushKey(BYTE keyNumber)
{
	if (keys[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::GetJoystickState()
{
	
	// コントローラーの状態を取得
	result = XInputGetState(0, &joyState);

	if (result == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

bool Input::PushJoyButton(uint32_t button)
{
	if (joyState.Gamepad.wButtons & button) {
		return true;
	}
	return false;
}

bool Input::TriggerJoyButton(uint32_t button)
{
	if ((joyState.Gamepad.wButtons & button) && ((preJoyState.Gamepad.wButtons & button) == 0)) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (keys[keyNumber] && preKeys[keyNumber] == 0) {
		return true;
	}
	return false;
}
Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

float Input::JoyStickParmLX(float num)
{
	// スティックの入力値を取得
	float lx = (float)joyState.Gamepad.sThumbLX / SHRT_MAX;

	// デッドゾーンの適用
	if (fabs(lx) < DEADZONE_THRESHOLD) lx = 0.0f;


	lx = lx * num;

	return lx;
}

float Input::JoyStickParmLY(float num)
{
	float ly = (float)joyState.Gamepad.sThumbLY / SHRT_MAX;

	// デッドゾーンの適用
	if (fabs(ly) < DEADZONE_THRESHOLD) ly = 0.0f;

	ly = ly * num;

	return ly;
}

float Input::JoyStickParmRX(float num)
{
	float rx = (float)joyState.Gamepad.sThumbRX / SHRT_MAX;

	if (fabs(rx) < DEADZONE_THRESHOLD) rx = 0.0f;
	rx = rx * num;

	return rx;
}
float Input::JoyStickParmRY(float num)
{
	float ry = (float)joyState.Gamepad.sThumbRY / SHRT_MAX;

	if (fabs(ry) < DEADZONE_THRESHOLD) ry = 0.0f;
	ry = ry * num;

	return ry;
}

