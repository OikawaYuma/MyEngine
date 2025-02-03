#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
#include "Object3dManager.h"
void TitleScene::Init()
{
	Object3dManager::GetInstance()->Init();
	DeleteObject();
	titleSpritePos_ = { 640.0f, 120.0f };
	titleSpriteVelo_ = 0.75f;
	sprite = new Sprite();
	sprite->Init(
		titleSpritePos_, { 700, 280 },
		{0.5f,0.5f},{1.0f,1.0f,1.0,1.0},
		"Resources/noise1.png");
	titleTex_ = TextureManager::StoreTexture("Resources/slimeTitle3.png");
	
	startTimer = 0;
	startTimer2 = 0;


	pushSpriteAlpha_ = 1.0f;
	pushSpriteAlphaPorM_ = 0.05f;
	pushASp_ = new Sprite();
	pushASp_->Init(
		{ 640.0f, 620.0f }, { 512, 128 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,pushSpriteAlpha_ },
		"Resources/noise1.png");
	pushATex_ = TextureManager::StoreTexture("Resources/pushA.png");

	loading_ = std::make_unique<GameLoading>();
	loading_->Init(JUMPSTART);

	camera_ = std::make_unique<TitleCamera>();
	camera_->Init();
	
	player_ = std::make_unique<TitlePlayer>();
	player_->Init();
	player2_ = std::make_unique<Player>();
	ground_ = std::make_unique<Ground>();
	Loder::LoadJsonFile("Resources/json", "titleStage2", player2_.get(), enemys_, items_, worldDesigns_,ground_.get());

	ground_->SetCamera(camera_->GetCamera());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();
	
	/////////////////////////////////////////////////
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera_->GetCamera());
	postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
	postProcess_->Init();
	thre_ = 0.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline);


	titleBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/title.wav");
	Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), titleBGM_, true);

	// シーン遷移Flag
	GamePlayFlag_ = false;
	Object3dManager::ObjectSort();
}

void TitleScene::Update()
{

	postProcess_->SerDepthOutlineInfo({ .farClip = depthOutlineInfo_.farClip,.diffSize = depthOutlineInfo_.diffSize });
	postProcess_->SerDissolveOutline({ .projectionInverse = Inverse(camera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = {1.0f,1.0f,1.0f} , .weightSize = 100 });
	if (camera_->GetTimer() >= 200) {
		threFlag_ = true;
	}
	if (camera_->GetTimer() >= 239) {
		player_->SetTranslate({ 0.0f, 0.5f, 100.0f });
	}
	
	if (threFlag_) {
		if (thre_ >= 1.2f && !GamePlayFlag_) {
			threPorM_ *= -1.0f;
			
		}
		
		thre_ += threPorM_;
		if (thre_ <= 0.0f) {
			threFlag_ = false;
			threPorM_ *= -1.0f;
		}
		
	}
	
	/*ImGui::Begin("thre");
	ImGui::DragFloat("ss",&thre_,0.01f);
	ImGui::End();*/
	postProcess_->SetThreshold(thre_);
	if (Input::GetInstance()->GetJoystickState()) {}
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A)) {
		GamePlayFlag_ = true;
		threPorM_ = 0.025f;
		threFlag_ = true;
	}
	else if(Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		GamePlayFlag_ = true;
		threPorM_ = 0.025f;
		threFlag_ = true;

	}
	
	if (titleSpritePos_.y >= 130.0f) {
		titleSpriteVelo_ *= -1.0f;
	}
	else if(titleSpritePos_.y <= 110.0f)
	{
		titleSpriteVelo_ *= -1.0f;
	}
	titleSpritePos_.y += titleSpriteVelo_;
	sprite->SetPosition(titleSpritePos_);
	sprite->Update();

	if (pushSpriteAlpha_ >= 1.0f) {
		pushSpriteAlphaPorM_ *= -1.0f;
	}
	else if (pushSpriteAlpha_ <= 0.0f)
	{
		pushSpriteAlphaPorM_ *= -1.0f;
	}
	pushSpriteAlpha_ += pushSpriteAlphaPorM_;
	sprite->SetColor({1.0f,1.0f,1.0f,pushSpriteAlpha_});
	pushASp_->Update();
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
	if (thre_ >= 1.2f && GamePlayFlag_) {
		loading_->Update();
		if (loading_->GetLoadingEffectEnd()) {

			IScene::SetSceneNo(STAGE);
			Audio::SoundStopWave(titleBGM_);

		}
	}
	Object3dManager::GetInstance()->Update();
	postProcess_->Update();
	
}
void TitleScene::Draw()
{
	skydome_->Draw(camera_->GetCamera());

	Object3dManager::GetInstance()->Draw(camera_->GetCamera());
	ground_->Draw();
	sprite->Draw(titleTex_, { 1.0f,1.0f,1.0f,1.0f });
	pushASp_->Draw(pushATex_, { 1.0f,1.0f,1.0f,pushSpriteAlpha_ });

	//player_->Draw(camera_->GetCamera());
	
}

void TitleScene::PostDraw()
{
	postProcess_->Draw();
	
}

void TitleScene::Draw2d()
{
	loading_->Draw();
}

void TitleScene::Release() {

}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}

void TitleScene::DeleteObject()
{
	enemys_.clear();

	items_.clear();

	worldDesigns_.clear();
}
