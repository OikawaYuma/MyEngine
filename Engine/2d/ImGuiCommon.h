/**
* @ file　ImGuiCommon.h
* @ brief ImGuiの使うための機能をまとめるクラス　Singleton Patternで作成
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx12.h"

class WinAPI;
class DirectXCommon;
class ImGuiCommon
{
public:
	static ImGuiCommon* GetInstance();

	ImGuiCommon() = default;
	~ImGuiCommon() = default;
	const ImGuiCommon& operator=(const ImGuiCommon&) = delete;

	void Initialize();
	void Update();
	void Draw();

	void UIParameter();
	void UICreate();
	void Release();

private:

	WinAPI* sWinAPI_ = nullptr;
	DirectXCommon* sDirectXCommon_ = nullptr;
};

