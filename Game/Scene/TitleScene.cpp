#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"

void TitleScene::Init()
{
	sprite = new Sprite();
	sprite->Init(
		{0, 0}, { 1280, 720 },
		{0,0},{1.0f,1.0f,1.0,1.0},
		"Resources/noise1.png");
	titleTex_ = TextureManager::StoreTexture("Resources/title.png");

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_.get());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kBloom);
}

void TitleScene::Update()
{
	if (Input::GetInstance()->GetJoystickState()) {}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A)) {
		IScene::SetSceneNo(STAGE);
	}
	else if(Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		IScene::SetSceneNo(STAGE);
	}
	
	postProcess_->Update();
	
}
void TitleScene::Draw()
{
	sprite->Draw(titleTex_, { 1.0f,1.0f,1.0,1.0 });
}

void TitleScene::PostDraw()
{
	postProcess_->Draw();
}

void TitleScene::Draw2d()
{
}

void TitleScene::Release() {

}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}