#include "GameOverScene.h"

#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
void GameOverScene::Init()
{
	sprite = new Sprite();
	sprite->Init(
		{ 0, 0 }, { 1280, 720 },
		{ 0,0 }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	titleTex_ = TextureManager::StoreTexture("Resources/gameOver.png");

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_.get());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kBloom);
}
void GameOverScene::Update()
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
void GameOverScene::Draw()
{
	sprite->Draw(titleTex_, { 1.0f,1.0f,1.0,1.0 });
}

void GameOverScene::Draw2d()
{
}

void GameOverScene::PostDraw()
{
	postProcess_->Draw();
}

void GameOverScene::Release() {

}


// ゲームを終了
int GameOverScene::GameClose()
{
	return false;
}
