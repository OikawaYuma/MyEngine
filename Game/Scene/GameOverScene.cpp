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
	titleTex_ = TextureManager::StoreTexture("Resources/GameOverSkydome.png");

	camera_ = std::make_unique<GameOverCamera>();
	camera_->Init();
	ground_ = std::make_unique<Ground>();
	ground_->Init();
	ground_->SetCamera(camera_->GetCamera());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();
	skydome_->SetSkydomeTexture(titleTex_);
	player_ = std::make_unique<Player>();
	player_->SetCamera(camera_->GetCamera());

	Loder::LoadJsonFile("Resources/json", "gameOverStage", player_.get(), enemys_, items_, worldDesigns_);
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
	skydome_->Update();
	camera_->Update();
	player_->TitleUpdate();
	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->GameOverUpdate();
	}
	for (std::list<std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}


	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Update();
	}
	camera_->Update();
	postProcess_->Update();

}
void GameOverScene::Draw()
{
	skydome_->Draw(camera_->GetCamera());
	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}

	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	for (std::list< std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	ground_->Draw();
	player_->Draw(camera_->GetCamera());
	//sprite->Draw(titleTex_, { 1.0f,1.0f,1.0,1.0 });
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
