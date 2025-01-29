/**
* @ file GuideUI.h
* @ brief Playerの行動に関するButton操作のUI設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include "Sprite.h"
#include "Camera.h"
class GuideUI
{
public:
	void Init();
	void Update();
	void Draw();

private:
	std::unique_ptr<Sprite> attackUI_ = nullptr;
	uint32_t attackUITex_ = 0;
	Vector4 attackColor_{};

	std::unique_ptr<Sprite> jumpUI_ = nullptr;
	uint32_t jumpUITex_ = 0;
	Vector4 jumpColor_{};
};

