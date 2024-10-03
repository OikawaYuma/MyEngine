#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"


void TitleScene::Init()
{
	sprite = new Sprite();
	sprite->Init(
		{640.0f, 120.0f}, { 1280, 312 },
		{0.5f,0.5f},{1.0f,1.0f,1.0,1.0},
		"Resources/noise1.png");
	titleTex_ = TextureManager::StoreTexture("Resources/slimeTitle3.png");

	pushASp_ = new Sprite();
	pushASp_->Init(
		{ 640.0f, 620.0f }, { 512, 128 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	pushATex_ = TextureManager::StoreTexture("Resources/pushA.png");

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({0.0f,5.0f,0.0f});


	ground_ = std::make_unique<Ground>();
	ground_->Init();
	ground_->SetCamera(camera_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	player_ = std::make_unique<TitlePlayer>();
	player_->Init();

	/////////////////////////////////////////////////
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_.get());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kFullScreen);
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
	skydome_->Update();
	camera_->Update();
	player_->Update();
	postProcess_->Update();
	
}
void TitleScene::Draw()
{
	skydome_->Draw(camera_.get());
	ground_->Draw();
	player_->Draw(camera_.get());
}

void TitleScene::PostDraw()
{
	postProcess_->Draw();
}

void TitleScene::Draw2d()
{
	sprite->Draw(titleTex_, {1.0f,1.0f,1.0f,1.0f});
	pushASp_->Draw(pushATex_, { 1.0f,1.0f,1.0f,1.0f });
}

void TitleScene::Release() {

}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}