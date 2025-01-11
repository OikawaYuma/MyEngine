#include "GuideUI.h"
#include "Input.h"
#include "TextureManager.h"
void GuideUI::Init()
{
	attackUI_ = std::make_unique<Sprite>();
	attackUI_->Init({1180.0f,500.0f},{84.0f,84.0f},{0.5f,0.5f},attackColor_, "Resources/player.png");
	attackUITex_ = TextureManager::GetInstance()->StoreTexture("Resources/AttackUI.png");
	attackColor_ = { 1.0f,1.0f,1.0f,1.0f };

	jumpUI_ = std::make_unique<Sprite>();
	jumpUI_->Init({ 1070.0f,620.0f }, { 72.0f,72.0f }, { 0.5f,0.5f }, attackColor_, "Resources/player.png");
	jumpUITex_ = TextureManager::GetInstance()->StoreTexture("Resources/JumpUI.png");
	jumpColor_ = { 1.0f,1.0f,1.0f,1.0f };
}

void GuideUI::Update()
{
	if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		attackColor_ = { 0.5f,0.5f,0.5f,1.0f };
	}
	else {
		attackColor_ = { 1.0f,1.0f,1.0f,1.0f };
	}

	if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_B)) {
		jumpColor_ = { 0.5f,0.5f,0.5f,1.0f };
	}
	else {
		jumpColor_ = { 1.0f,1.0f,1.0f,1.0f };
	}
	attackUI_->Update();
	jumpUI_->Update();
}

void GuideUI::Draw()
{
	attackUI_->Draw(attackUITex_, attackColor_);
	jumpUI_->Draw(jumpUITex_, jumpColor_);
}
