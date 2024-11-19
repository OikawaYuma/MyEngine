#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "Loder.h"
#include "Audio.h"
void TitleScene::Init()
{
	titleSpritePos_ = { 640.0f, 120.0f };
	titleSpriteVelo_ = 0.75f;
	sprite = new Sprite();
	sprite->Init(
		titleSpritePos_, { 700, 280 },
		{0.5f,0.5f},{1.0f,1.0f,1.0,1.0},
		"Resources/noise1.png");
	titleTex_ = TextureManager::StoreTexture("Resources/slimeTitle3.png");

	LoadStringSp_ =  std::make_unique<Sprite>();
	loadpos = 800;
	LoadStringSp_->Init(
		{800,loadpos }, { 500, 120 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	LoadStringSpTex_ = TextureManager::StoreTexture("Resources/LoadString.png");

	slime2DSp1_ = std::make_unique<Slime2d>();
	slime2DSp1_->Init(
		{1050,800 },0.2f,2,true);

	slime2DSp2_ = std::make_unique<Slime2d>();
	slime2DSp2_->Init(
		{ 1135,800 },0.2f,3,false);

	slime2DSp3_ = std::make_unique<Slime2d>();
	slime2DSp3_->Init(
		{ 1220,800 },0.25f,4,false);

	jumpNum_ = JUMPSTART;
	
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

	camera_ = std::make_unique<TitleCamera>();
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
	postProcess_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
	postProcess_->Init();
	thre_ = 0.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;
	IPostEffectState::SetEffectNo(PostEffectMode::kDepthOutline);

	loadingSpriteMoveFlag_ = false;

	titleBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/title.wav");
	Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), titleBGM_, true);

	// シーン遷移Flag
	GamePlayFlag_ = false;
}

void TitleScene::Update()
{

	ImGui::Begin("OBB,BALL");


	ImGui::DragFloat("sColor", &depthOutlineInfo_.farClip, 0.0001f);
	ImGui::DragFloat2("sde", &depthOutlineInfo_.diffSize.x, 0.01f);
	//ImGui::DragFloat3("sDire", &spotLight_.direction.x, 0.1f);
	//ImGui::DragFloat3("sPos", &spotLight_.position.x, 0.1f);
	//ImGui::DragFloat("sDis", &spotLight_.distance, 0.1f);
	//ImGui::DragFloat("sInten", &spotLight_.intensity, 0.1f);
	//ImGui::DragFloat("sDacya", &spotLight_.dacya, 0.1f);
	//ImGui::DragFloat("scosAngle", &cosAngle_, 0.1f);
	//ImGui::Text("playerPosX %f", spotLight_.position.x);
	//ImGui::Text("playerPosZ %f", spotLight_.position.z);

	ImGui::End();
	postProcess_->SerDepthOutlineInfo({ .farClip = depthOutlineInfo_.farClip,.diffSize = depthOutlineInfo_.diffSize });
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
	player_->TitleUpdate();
	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Update();
		/*
		//// enemy->Fire();
		//if ((*itr)->GetFireTimer() >= (*itr)->kFireInterval) {
		//	assert(player_);
		//	// 弾の速度
		//	const float kBulletSpeed = 1.0f;

		//	Vector3 start = (*itr)->GetWorldPosition();
		//	Vector3 end = player_->GetWorldPosition();

		//	Vector3 diffVector;
		//	diffVector.x = end.x - start.x;
		//	diffVector.y = end.y - start.y;
		//	diffVector.z = end.z - start.z;

		//	diffVector = Normalize(diffVector);
		//	diffVector.x *= kBulletSpeed;
		//	diffVector.y *= kBulletSpeed;
		//	diffVector.z *= kBulletSpeed;

		//	Vector3 velocity(diffVector.x, diffVector.y, diffVector.z);

		//	// 速度ベクトルを自機の向きに合わせて回転させる
		//	velocity = TransformNormal(velocity, (*itr)->GetWorldTransform().matWorld_);

		//	// 弾を生成し、初期化
		//	EnemyBullet* newBullet = new EnemyBullet();
		//	newBullet->Init((*itr)->GetWorldTransform().translation_, velocity);
		//	newBullet->SetPlayer(player_.get());
		//	// 弾を登録する
		//	enemyBullets_.push_back(newBullet);
		//	(*itr)->SetFireTimer(0);
		//}
		*/
	}
	for (std::list<std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}


	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Update();
	}
	if (thre_ >= 1.2f && GamePlayFlag_) {
		switch (jumpNum_) {
		case JUMPONE: {
			slime2DSp1_->Update();
			if (!slime2DSp1_->GetIsJump()) {
				jumpNum_ = JUMPTWO;
				slime2DSp2_->SetIsJump(true);
			}
			break;
		}
		case JUMPTWO: {
			slime2DSp2_->Update();
			if (!slime2DSp2_->GetIsJump()) {
				jumpNum_ = JUMPTHREE;
				slime2DSp3_->SetIsJump(true);
			}
			break;
		}
		case JUMPTHREE: {
			slime2DSp3_->Update();
			if (!slime2DSp3_->GetIsJump()) {
				jumpNum_ = JUMPONE;
				slime2DSp1_->SetIsJump(true);
			}
			break;
		}
		case JUMPSTART:
		{
			
				if (slime2DSp1_->IsApear()) {
					loadpos -= 4.0f;
				}
				
				slime2DSp1_->StartUpdate();
				slime2DSp2_->StartUpdate();
				slime2DSp3_->StartUpdate();

				if (!slime2DSp1_->IsApear() && loadpos <= 660.0f) {
					loadpos += 4.0f;
					if (loadpos >= 660.0f) {
						loadpos = 660.0f;
					}
				}


				LoadStringSp_->SetPosition({ 800.0f,loadpos });
				LoadStringSp_->Update();

				if (slime2DSp1_->IsStart()) {


					IScene::SetSceneNo(STAGE);
					DeleteObject();
					Audio::SoundStopWave(titleBGM_);

				}
			
			break;
		}


		}
	}
	
	postProcess_->Update();
	
}
void TitleScene::Draw()
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
	sprite->Draw(titleTex_, { 1.0f,1.0f,1.0f,1.0f });

	pushASp_->Draw(pushATex_, { 1.0f,1.0f,1.0f,pushSpriteAlpha_ });
}

void TitleScene::PostDraw()
{
	postProcess_->Draw();
	
}

void TitleScene::Draw2d()
{
	if (GamePlayFlag_ && thre_ >= 1.0f) {
		LoadStringSp_->Draw(LoadStringSpTex_, { 1.0f,1.0f,1.0f,1.0f });
		slime2DSp1_->Draw();
		slime2DSp2_->Draw();
		slime2DSp3_->Draw();
	}
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
