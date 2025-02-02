#include "ClearScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include <Loder.h>
#include <Audio.h>
#include "Object3dManager.h"
void ClearScene::Init()
{
	Object3dManager::GetInstance()->Init();
	// 全シーンのObjectのリストを空にする
	DeleteObject();
	clearColorTex_ = TextureManager::StoreTexture("Resources/GameClearSkydome.png");
	// Camera初期化
	camera_ = std::make_unique<ClearCamera>();
	camera_->Init();
	// Skydome初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();
	skydome_->SetSkydomeTexture(clearColorTex_);
	// Player初期化
	player_ = std::make_unique<ClearPlayer>();
	player_->Init();
	player2_ = std::make_unique<Player>();
	// Ground初期化
	ground_ = std::make_unique<Ground>();
	ground_->SetCamera(camera_->GetCamera());

	// Loderによる読み込み
	Loder::LoadJsonFile("Resources/json", "gameClear2", player2_.get(), enemys_, items_, worldDesigns_, ground_.get());

	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->Init();
	postProcess_->SerDissolveOutline({ .projectionInverse = Inverse(camera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = { 1.0f, 0.984313f, 0.643f } , .weightSize = 100 });
	postProcess_->SetDissolveInfo({ 1.0f, 0.984313f, 0.643f });
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline); // OutlineとDissolveの使用
	thre_ = 1.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;
	postProcess_->SetThreshold(thre_);

	clearFlag_ = false;

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
	// ClearBerUI
	clearBerYPos = 110.0f;
	clearBerYPosPram = 0.125f;
	clearBer_ = std::make_unique<Sprite>();
	clearBer_->Init(
		{ 360.0f, clearBerYPos }, { 780, 160 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,pushSpriteAlpha_ },
		"Resources/noise1.png");
	clearTex_ = TextureManager::StoreTexture("Resources/GameClear3.png");

	// Audio
	clearBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/kasumisou.wav");
	Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), clearBGM_, true);
	pushSE_ = Audio::GetInstance()->SoundLoadWave("Resources/slimePush.wav");

	// 透明、不透明の条件でソートを掛ける
	Object3dManager::ObjectSort();
}
void ClearScene::Update()
{
	
	skydome_->Update();
	camera_->Update();
	player_->Update();
	for (std::list<std::unique_ptr<BaseEnemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Update();
	}
	for (std::list<std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}


	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Update();
	}

	if (Input::GetInstance()->GetJoystickState()) {}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A) && threFlag_) {
		clearFlag_ = true;
		postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
		Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), pushSE_, false);
		//threPorM_ = 0.025f;
	}
	clearBerYPos += clearBerYPosPram;
	if (clearBerYPos >= 125.0f) {
		clearBerYPosPram *= -1.0f;
	}
	else if (clearBerYPos <= 100.0f) {
		clearBerYPosPram *= -1.0f;
	}
	clearBer_->SetPosition({ 640.0f,clearBerYPos });


	if (!threFlag_) {
		thre_ -= threPorM_;
		if (thre_ <= 0.0f) {

			threPorM_ *= -1.0f;
			threFlag_ = true;
		}
	}
	else if (threFlag_ && clearFlag_) {
		thre_ -= threPorM_;
		Audio::GetInstance()->SoundStopWave(clearBGM_);
		if (thre_ >= 1.2f) {
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
	postProcess_->SerDissolveOutline({ .projectionInverse = Inverse(camera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = { 1.0f, 0.984313f, 0.643f } , .weightSize = 100 });
	pushSpriteAlpha_ += pushSpriteAlphaPorM_;
	//sprite->SetColor({ 1.0f,1.0f,1.0f,pushSpriteAlpha_ });
	pushASp_->Update();
	clearBer_->Update();
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();
	
}
void ClearScene::Draw()
{
	// 3D
	skydome_->Draw(camera_->GetCamera());
	ground_->Draw();
	// マネジャーに積んだObjectのDraw
	Object3dManager::GetInstance()->Draw(camera_->GetCamera());
	// 2D
	pushASp_->Draw(pushATex_, { 1.0f,1.0f,1.0f,pushSpriteAlpha_ });
	clearBer_->Draw(clearTex_, { 1.0f,1.0f,1.0f,1.0f });
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
