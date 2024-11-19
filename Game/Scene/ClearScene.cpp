#include "ClearScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include <Loder.h>
#include <Audio.h>
void ClearScene::Init()
{
	titleTex_ = TextureManager::StoreTexture("Resources/GameClearSkydome.png");

	camera_ = std::make_unique<ClearCamera>();
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

	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->ClearInit();
	}
	player_->ClearInit();
	/////////////////////////////////////////////////

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();

	postProcess_->SetDissolveInfo({ 1.0f, 0.984313f, 0.643f });
	IPostEffectState::SetEffectNo(PostEffectMode::kDissolve);

	thre_ = 1.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;

	postProcess_->SetThreshold(thre_);
	titleFlag_ = false;

	pushSpriteAlpha_ = 1.0f;
	pushSpriteAlphaPorM_ = 0.025f;
	pushASp_ = std::make_unique<Sprite>();
	pushASp_->Init(
		{ 640.0f, 620.0f }, { 512, 128 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,pushSpriteAlpha_ },
		"Resources/noise1.png");

	pushATex_ = TextureManager::StoreTexture("Resources/pushA.png");

	titleBerYPos = 110.0f;
	titleBerYPosPram = 0.125f;
	titleBer_ = std::make_unique<Sprite>();
	titleBer_->Init(
		{ 360.0f, titleBerYPos }, { 780, 160 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,pushSpriteAlpha_ },
		"Resources/noise1.png");
	gameOverTex_ = TextureManager::StoreTexture("Resources/GameClear3.png");

	gameOverBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/kasumisou.wav");
	Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), gameOverBGM_, true);
	pushSE_ = Audio::GetInstance()->SoundLoadWave("Resources/slimePush.wav");

}
void ClearScene::Update()
{

	skydome_->Update();
	camera_->Update();
	player_->ClearUpdate();
	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->ClearUpdate();
	}
	for (std::list<std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}


	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Update();
	}

	if (Input::GetInstance()->GetJoystickState()) {}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A) && threFlag_) {
		titleFlag_ = true;
		postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
		Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), pushSE_, false);
		//threPorM_ = 0.025f;
	}
	titleBerYPos += titleBerYPosPram;
	if (titleBerYPos >= 125.0f) {
		titleBerYPosPram *= -1.0f;
	}
	else if (titleBerYPos <= 100.0f) {
		titleBerYPosPram *= -1.0f;
	}
	titleBer_->SetPosition({ 640.0f,titleBerYPos });


	if (!threFlag_) {
		thre_ -= threPorM_;
		if (thre_ <= 0.0f) {

			threPorM_ *= -1.0f;
			threFlag_ = true;
		}
	}
	else if (threFlag_ && titleFlag_) {
		thre_ -= threPorM_;
		Audio::GetInstance()->SoundStopWave(gameOverBGM_);
		if (thre_ >= 1.2f) {
			DeleteObject();
			IScene::SetSceneNo(TITLE);
		}

	}

	if (pushSpriteAlpha_ >= 1.0f) {
		pushSpriteAlphaPorM_ *= -1.0f;
	}
	else if (pushSpriteAlpha_ <= 0.0f)
	{
		pushSpriteAlphaPorM_ *= -1.0f;
	}
	postProcess_->SetThreshold(thre_);
	pushSpriteAlpha_ += pushSpriteAlphaPorM_;
	//sprite->SetColor({ 1.0f,1.0f,1.0f,pushSpriteAlpha_ });
	pushASp_->Update();
	titleBer_->Update();
	postProcess_->Update();
	
}
void ClearScene::Draw()
{
	skydome_->Draw(camera_->GetCamera());
	ground_->Draw();
	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->ClearDraw(camera_->GetCamera());
	}

	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}
	for (std::list< std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Draw(camera_->GetCamera());
	}

	player_->Draw(camera_->GetCamera());
	pushASp_->Draw(pushATex_, { 1.0f,1.0f,1.0f,pushSpriteAlpha_ });
	titleBer_->Draw(gameOverTex_, { 1.0f,1.0f,1.0f,1.0f });
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

void ClearScene::DeleteObject()
{

	enemys_.clear();

	items_.clear();

	worldDesigns_.clear();
}
