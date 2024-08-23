#include "ClearScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
void ClearScene::Init()
{
	sprite = new Sprite();
	sprite->Init(
		{ 0, 0 }, { 1280, 720 },
		{ 0,0 }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	titleTex_ = TextureManager::StoreTexture("Resources/clear.png");

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_.get());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kFullScreen);
}
void ClearScene::Update()
{
	if (Input::GetInstance()->GetJoystickState()) {}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A)) {
		IScene::SetSceneNo(TITLE);
	}
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		IScene::SetSceneNo(TITLE);
	}

	postProcess_->Update();
	
}
void ClearScene::Draw()
{
	sprite->Draw(titleTex_, { 1.0f,1.0f,1.0,1.0 });
}

void ClearScene::Draw2d()
{
}

void ClearScene::PostDraw()
{
	postProcess_->Draw();
}

void ClearScene::Release() {

}


// ゲームを終了
int ClearScene::GameClose()
{
	return false;
}