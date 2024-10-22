#include "GameOverScene.h"

#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include <Loder.h>
void GameOverScene::Init()
{
	sprite = new Sprite();
	sprite->Init(
		{ 0, 0 }, { 1280, 720 },
		{ 0,0 }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	titleTex_ = TextureManager::StoreTexture("Resources/gameOver.png");

	camera_ = std::make_unique<GameOverCamera>();
	camera_->Init();
	ground_ = std::make_unique<Ground>();
	ground_->Init();
	ground_->SetCamera(camera_->GetCamera());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	player_ = std::make_unique<Player>();
	player_->SetCamera(camera_->GetCamera());

	Loder::LoadJsonFile("Resources/json", "titleStage", player_.get(), enemys_, items_, worldDesigns_);
	player_->TitleInit();
	/////////////////////////////////////////////////

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	IPostEffectState::SetEffectNo(PostEffectMode::kDissolve);
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
