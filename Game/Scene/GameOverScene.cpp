#include "GameOverScene.h"

#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include <Loder.h>
#include <Audio.h>
#include "Object3dManager.h"
void GameOverScene::Init()
{
	Object3dManager::GetInstance()->Init();
	DeleteObject();
	gameOverColorTex_ = TextureManager::StoreTexture("Resources/GameOverSkydome2.png");
	// Camera初期化
	camera_ = std::make_unique<GameOverCamera>();
	camera_->Init();
	// Skydome初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();
	skydome_->SetSkydomeTexture(gameOverColorTex_);
	// Player初期化
	player_ = std::make_unique<GameOverPlayer>();
	player_->Init();
	player2_ = std::make_unique<Player>();
	// Ground初期化
	ground_ = std::make_unique<Ground>();
	ground_->SetCamera(camera_->GetCamera());

	// Loderによる読み込み
	Loder::LoadJsonFile("Resources/json", "gameOverStage2", player2_.get(), enemys_, items_, worldDesigns_,ground_.get());

	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	postProcess_->SerDissolveOutline({ .projectionInverse = Inverse(camera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = { 0.35f, 0.025f, 0.025f } , .weightSize = 100 });
	postProcess_->SetDissolveInfo({ 0.35f, 0.025f, 0.025f });
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline); // OutlineとDissolveの使用
	thre_ = 1.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;
	postProcess_->SetThreshold(thre_);

	gameOverFlag_ = false;

	// UIの初期化
	// PushUI
	pushSpriteAlpha_ = 1.0f;
	pushSpriteAlphaPorM_ = 0.025f;
	pushASp_ = std::make_unique<Sprite>();
	pushASp_->Init(
		{ 640.0f, 620.0f }, { 512, 128 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,pushSpriteAlpha_ },
		"Resources/noise1.png");
	pushATex_ = TextureManager::StoreTexture("Resources/pushA.png");
	// GameOverBerUI
	gameOverBerYPos = 200.0f;
	gameOverBerYPosPram = 0.125f;
	titleBer_ = std::make_unique<Sprite>();
	titleBer_->Init(
		{ 965.0f, gameOverBerYPos }, { 600, 300 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,pushSpriteAlpha_ },
		"Resources/noise1.png");
	gameOverTex_ = TextureManager::StoreTexture("Resources/GameOver2.png");

	// Audio
	gameOverBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/gameOver.wav");
	Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), gameOverBGM_, true);
	pushSE_= Audio::GetInstance()->SoundLoadWave("Resources/slimePush.wav");

	// 透明、不透明の条件でソートを掛ける
	Object3dManager::ObjectSort();
}
void GameOverScene::Update()
{
	
	skydome_->Update();
	camera_->Update();
	for (std::list<std::unique_ptr<BaseEnemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Update();
	}
	for (std::list<std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}


	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Update();
	}
	camera_->Update();

	if (Input::GetInstance()->GetJoystickState()) {}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A) && threFlag_) {
		gameOverFlag_ = true;
		postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
		Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), pushSE_, false);
		//threPorM_ = 0.025f;
	}
	gameOverBerYPos += gameOverBerYPosPram;
	if (gameOverBerYPos >= 210.0f) {
		gameOverBerYPosPram *= -1.0f;
	}
	else if (gameOverBerYPos <=190.0f) {
		gameOverBerYPosPram *= -1.0f;
	}
	titleBer_->SetPosition({965.0f,gameOverBerYPos});
	
	
	if (!threFlag_) {
		thre_ -= threPorM_;
		if (thre_ <= 0.0f) {

			threPorM_ *= -1.0f;
			threFlag_ = true;
		}
	}
	else if (threFlag_ && gameOverFlag_) {
		thre_ -= threPorM_;
		Audio::GetInstance()->SoundStopWave(gameOverBGM_);
		if (thre_ >= 1.2f) {
			//DeleteObject();
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
	postProcess_->SerDissolveOutline({ .projectionInverse = Inverse(camera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = { 0.35f, 0.025f, 0.025f } , .weightSize = 100 });
	postProcess_->SetThreshold(thre_);
	pushSpriteAlpha_ += pushSpriteAlphaPorM_;
	pushASp_->Update();
	titleBer_->Update();
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();

}
void GameOverScene::Draw()
{
	// 3D
	skydome_->Draw(camera_->GetCamera());
	ground_->Draw();
	// マネジャーに積んだObjectのDraw
	Object3dManager::GetInstance()->Draw(camera_->GetCamera());
	pushASp_->Draw(pushATex_, { 1.0f,1.0f,1.0f,pushSpriteAlpha_ });
	// 2D
	titleBer_->Draw(gameOverTex_,{1.0f,1.0f,1.0f,1.0f});
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

void GameOverScene::DeleteObject()
{
	enemys_.clear();

	items_.clear();

	worldDesigns_.clear();
}
