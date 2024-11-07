#include "ClearScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include <Audio.h>
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

	postProcess_->SetDissolveInfo({ 1.0f, 1.0f, 1.0f });
	IPostEffectState::SetEffectNo(PostEffectMode::kDissolve);

	thre_ = 1.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;

	postProcess_->SetThreshold(thre_);
	titleFlag_ = false;
}
void ClearScene::Update()
{
	


	if (Input::GetInstance()->GetJoystickState()) {}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A) && threFlag_) {
		titleFlag_ = true;
		postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
		//Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), pushSE_, false);
		//threPorM_ = 0.025f;
	}



	if (!threFlag_) {
		thre_ -= threPorM_;
		if (thre_ <= 0.0f) {

			threPorM_ *= -1.0f;
			threFlag_ = true;
		}
	}
	else if (threFlag_ && titleFlag_) {
		thre_ -= threPorM_;
		//Audio::GetInstance()->SoundStopWave(gameOverBGM_);
		if (thre_ >= 1.2f) {
			IScene::SetSceneNo(TITLE);
		}

	}
	postProcess_->SetThreshold(thre_);
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