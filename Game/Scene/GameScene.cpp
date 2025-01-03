#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
#include "Object3dManager.h"
#include <Audio.h>
void GameScene::Init()
{
	
	Object3dManager::GetInstance()->Init();
	DeleteObject();
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	player_ = std::make_unique<Player>();
	player_->Init({0.0f,1.0f,0.0f,},"player");
	player_->SetCamera(followCamera_->GetCamera());
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransform());

	ground_ = std::make_unique<Ground>();
	Loder::LoadJsonFile("Resources/json", "stage6", player_.get(), enemys_, items_, worldDesigns_, ground_.get());
	ground_->SetCamera(followCamera_->GetCamera());

	followCamera_->PosAdustment();

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Init();
	player_->SetLockOn(lockOn_.get());
	followCamera_->SetLockOn(lockOn_.get());
	
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	cosAngle_ = 3.0f;
	spotLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	spotLight_.position = { 2.0f ,1.25f,5.0 };
	spotLight_.distance = 7.0f;
	spotLight_.direction =
		Normalize(Vector3{ -1.0f,-1.0f,0.0f });
	spotLight_.intensity = 4.0f;
	spotLight_.dacya = 2.0f;
	spotLight_.cosAngle =
		std::cos(std::numbers::pi_v<float> / cosAngle_);
	
	gameStateMode_ = WAITGAME;
	jumpNum_ = JUMPONE;
	jumpRoopNum = 0;
	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	postProcess_->SetDissolveInfo({1.0f,1.0f,1.0f});
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline);
	postProcess_->SerDissolveOutline({ .weightSize = 100.0f });


	postProcess2_ = new PostProcess();
	postProcess2_->Init();
	postProcess2_->SetCamera(followCamera_->GetCamera());
	postProcess2_->SetDissolveInfo({ 1.0f,1.0f,1.0f });
	postProcess2_->SetEffectNo(PostEffectMode::kDissolve);

	thre_ = 1.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;
	postProcess_->SetThreshold(thre_);

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->SetGameScene(this);
	collisionManager_->SetPlayer(player_.get());
	destroyCount_ = 0;

	LoadStringSp_ = std::make_unique<Sprite>();
	LoadStringSp_->Init(
		{ 800,660 }, { 500, 120 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	LoadStringSpTex_ = TextureManager::StoreTexture("Resources/LoadString.png");

	moveflag1 = false;
	moveFlag2 = false;
	startSpritePos_ = { 640.0f , -250.0f };
	startSpriteVelo_ = 0.75f;
	startEffectSp_ = std::make_unique<Sprite>();
	startEffectSp_->Init(
		startSpritePos_, { 800, 280 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	startEffectSpTex_ = TextureManager::StoreTexture("Resources/StartSprite.png");

	startSpritePos2_ = { 640.0f , -250.0f };
	startSpriteVelo2_ = 0.75f;
	startEffectSp2_ = std::make_unique<Sprite>();
	startEffectSp2_->Init(
		startSpritePos2_, { 800, 280 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	startEffectSpTex2_ = TextureManager::StoreTexture("Resources/StartSprite2.png");


	statrTimer_ = 0;

	slime2DSp1_ = std::make_unique<Slime2d>();
	slime2DSp1_->Init(
		{ 1050,650 }, 0.2f, 2, true);

	slime2DSp2_ = std::make_unique<Slime2d>();
	slime2DSp2_->Init(
		{ 1135,650 }, 0.2f, 3, false);

	slime2DSp3_ = std::make_unique<Slime2d>();
	slime2DSp3_->Init(
		{ 1220,650 }, 0.25f, 4, false);

	jumpRoopNum = 0;
	loadpos = 660.0f;
	endTimer = 0;
	startTimer = 0;
	cameraFlag_ = false;

	gameBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/game.wav");
	GameOverFlag = false;
	slimeDeadSE_ = Audio::GetInstance()->SoundLoadWave("Resources/slimeDead.wav");
	gameClearSE_ = Audio::GetInstance()->SoundLoadWave("Resources/clearSE.wav");


	// スコア
	score_ = std::make_unique<Score>();
	score_->Init();

	killCount_ = 0;
	// ゲームタイマー
	gameTimer_ = std::make_unique<GameTimer>();
	gameTimer_->Init();

	// クリア条件の敵を倒した数の設定
	clearFlagCount_ = (int)enemys_.size();

	enemyApear_.Init();
	enemyApear_.SetPlayer(player_.get());
	
	Enemy::SetEnemydestory(0);
}

void GameScene::Update()
{
	
#ifdef DEBUG
	GlobalVariables::GetInstance()->Update();
#endif // DEBUG
	DissolveOutlineInfo dissolveOutlineInfo = postProcess_->GetDissolveOutline();
	/*ImGui::Begin("OBB,BALL");


	ImGui::DragFloat("weightSize", &dissolveOutlineInfo.weightSize, 0.0001f);

	ImGui::DragFloat3("sDire", &spotLight_.direction.x, 0.1f);
	ImGui::DragFloat3("sPos", &spotLight_.position.x, 0.1f);
	ImGui::DragFloat("sDis", &spotLight_.distance, 0.1f);
	ImGui::DragFloat("sInten", &spotLight_.intensity, 0.1f);
	ImGui::DragFloat("sDacya", &spotLight_.dacya, 0.1f);
	ImGui::DragFloat("scosAngle", &cosAngle_, 0.1f);
	ImGui::Text("playerPosX %f", spotLight_.position.x);
	ImGui::Text("playerPosZ %f", spotLight_.position.z);
	
	ImGui::End();*/
	postProcess_->SerDissolveOutline({.projectionInverse = Inverse(followCamera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = {1.0f,1.0f,1.0f} , .weightSize  = dissolveOutlineInfo.weightSize});
	spotLight_.cosAngle =
		std::cos(std::numbers::pi_v<float> / cosAngle_);
	spotLight_.direction = Normalize(spotLight_.direction);
	switch (gameStateMode_) {
	case WAITGAME:
	{
		switch (jumpNum_) {
		case JUMPONE: {
			startTimer++;
			if (startTimer >= 10.0f) {
				slime2DSp1_->Update();
				if (!slime2DSp1_->GetIsJump()) {
					jumpNum_ = JUMPTWO;
					slime2DSp2_->SetIsJump(true);
				}
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
				jumpRoopNum++;
				if (jumpRoopNum >= 2) {
					jumpNum_ = JUMPEND;
				}
			}
			break;
		}
		case JUMPEND:
		{
			
			endTimer++;
			if (endTimer >= 10) {
				if (slime2DSp1_->IsApear()) {
					loadpos -= 4.0f;
				}
				if (!slime2DSp1_->IsApear() && loadpos <= 800.0f) {
					loadpos += 4.0f;
				}
				slime2DSp1_->EndUpdate();
				slime2DSp2_->EndUpdate();
				slime2DSp3_->EndUpdate();




				LoadStringSp_->SetPosition({ 800.0f,loadpos });
				LoadStringSp_->Update();

				if (slime2DSp1_->IsStart()) {

					
					thre_ -= threPorM_;;
					if (thre_ <= 0.5f) {
						followCamera_->StartCameraEffect();
					}
					if (thre_ <= 0.0f) {
						gameStateMode_ = STARTGAME;
						threPorM_ = 0.025f;
						threFlag_ = true;
						Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), gameBGM_, true);
					}
					postProcess_->SetThreshold(thre_);
					//Audio::SoundStopWave(0);
					

				}
			}

			break;
		}
		}
		
	

	
		
		break;
	}
	case STARTGAME: {
		if (!cameraFlag_) {
			followCamera_->StartCameraEffect();
		}
		
		startTimer++;
		if (!moveFlag2) {
			
			if (!moveflag1) {
				startSpriteVelo_ = 4.0f;
				if (startSpritePos_.y >= 130.0f) {
					startSpriteVelo_ = 0.75f;
					startSpriteVelo_ *= -1.0f;
					moveflag1 = true;
				}
			}
			else if (moveflag1) {
				if (startSpritePos_.y >= 130.0f) {
					startSpriteVelo_ *= -1.0f;
				}
				else if (startSpritePos_.y <= 110.0f)
				{
					startSpriteVelo_ *= -1.0f;
				}
			}
			
			if (startTimer >= 300) {
				moveFlag2 = true;
			}
		}
		else if (moveFlag2) {
			startSpriteVelo_ = -5.0f;
		}

		if (startTimer >=360.0f) {
			if (threFlag_) {
				if (thre_ >= 1.2f) {
					threPorM_ *= -1.0f;
					cameraFlag_ = true;
					followCamera_->Upadate();
				}

				thre_ += threPorM_;
				if (thre_ <= 0.0f) {
					threFlag_ = false;
					threPorM_ *= -1.0f;
					gameStateMode_ = PLAYGAME;
					player_->CreateReticle();		
				}

			}
		}
		postProcess2_->SetThreshold(thre_);
		startSpritePos_.y += startSpriteVelo_;
		startEffectSp_->SetPosition(startSpritePos_);
		startEffectSp_->Update();
		break;
	}
	case PLAYGAME: 
	{
		gameTimer_->Start();
		enemyApear_.Update(enemys_);
		
		
		startSpritePos2_.y += 12.0f;
		startEffectSp2_->SetPosition(startSpritePos2_);
		startEffectSp2_->Update();
		
		score_->Update(gameTimer_->GetGameTime(), Enemy::GetEnemyDestory());
		/*items_.remove_if([](std::unique_ptr<PlayerItem>& bullet) {
			if (bullet->IsDead()) {
				return true;
			}
			return false;
			});*/

		// 現状のクリア条件
		if (gameTimer_->GetGameTime() <= 0) {

			Audio::SoundStopWave(gameBGM_);
			postProcess_->SetDissolveInfo({ 1.0f, 0.984313f, 0.643f });
			Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), gameClearSE_, false);
			gameStateMode_ = CLEARGAME;
			threPorM_ = -0.025f;

		}
		// 現状のゲームオーバー条件
		// PlayerのHpが０になったら
		if (player_->GetHP() <= 0.3f) {
			Audio::SoundStopWave(gameBGM_);
			Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), slimeDeadSE_, false);
			postProcess_->SetDissolveInfo({ 0.35f, 0.025f, 0.025f });
			gameStateMode_ = DEADGAME;
			threPorM_ = -0.025f;	
		}
		// ロックオン
		lockOn_->Update(enemys_, followCamera_->GetCamera(), player_.get());
		player_->SetSpotLight(spotLight_);
		player_->Update();
		followCamera_->Upadate();
		ground_->SetSpotLight(spotLight_);
		ground_->Update();
		skydome_->Update();
		// エネミーの弾発射処理
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
		for (std::list< std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
			(*itr)->Update();
		}


		for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
			(*itr)->SetSpotLight(spotLight_);
			(*itr)->Update();

		}
		Object3dManager::GetInstance()->Update();
		
		gameTimer_->Update(startSpritePos2_.y);
		collisionManager_->CheckAllCollision();
		break;
	}
	case CLEARGAME:
	{
		thre_ -= threPorM_;;
		if (thre_ >= 1.2f) {
			//DeleteObject();
			//Audio::GetInstance()->SoundStopWave(slimeDeadSE_);
			IScene::SetSceneNo(CLEAR);
		}

		postProcess_->SetThreshold(thre_);
		break;
	}
	case DEADGAME:
	{
		thre_ -= threPorM_;;
		if (thre_ >= 1.2f) {
			//DeleteObject();
			Audio::GetInstance()->SoundStopWave(slimeDeadSE_);
			IScene::SetSceneNo(GAMEOVER);
		}
		
		postProcess_->SetThreshold(thre_);
		break;
	}
	}
	
	
	
	
}
void GameScene::Draw()
{
	skydome_->Draw(followCamera_->GetCamera());
	ground_->Draw();
	player_->Draw(followCamera_->GetCamera());
	Object3dManager::GetInstance()->Draw(followCamera_->GetCamera());
	
	
	lockOn_->Draw();
	
}

void GameScene::Draw2d()
{
	
	
	switch (gameStateMode_) {
	case PLAYGAME:
		gameTimer_->Draw();
		score_->Draw();
		player_->DrawUI();
		break;
	}

	LoadStringSp_->Draw(LoadStringSpTex_, { 1.0f,1.0f,1.0f,1.0f });
	slime2DSp1_->Draw();
	slime2DSp2_->Draw();
	slime2DSp3_->Draw();
	startEffectSp_->Draw(startEffectSpTex_, { 1.0f,1.0f,1.0f,1.0f });
	startEffectSp2_->Draw(startEffectSpTex2_, { 1.0f,1.0f,1.0f,1.0f });
}

void GameScene::PostDraw()
{
	
	postProcess_->Draw();
	//postProcess2_->Draw();
	
}

void GameScene::Release() {
	postProcess_->Release();
}
// ゲームを終了
int GameScene::GameClose()
{
	return false;
}

void GameScene::DeleteObject()
{
	enemys_.clear();

	items_.clear();

	worldDesigns_.clear();
}

