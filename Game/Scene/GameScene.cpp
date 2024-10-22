#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
#include <Audio.h>
void GameScene::Init()
{
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	player_ = std::make_unique<Player>();
	player_->SetCamera(followCamera_->GetCamera());
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransform());
	Loder::LoadJsonFile("Resources/json", "stage", player_.get(),enemys_, items_, worldDesigns_);
	followCamera_->PosAdustment();

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Init();
	player_->SetLockOn(lockOn_.get());
	followCamera_->SetLockOn(lockOn_.get());
	ground_ = std::make_unique<Ground>();
	ground_->Init();
	ground_->SetCamera(followCamera_->GetCamera());
	
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	gameStateMode_ = WAITGAME;
	jumpNum_ = JUMPONE;
	jumpRoopNum = 0;
	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	IPostEffectState::SetEffectNo(PostEffectMode::kDissolve);
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
	
}

void GameScene::Update()
{
	
#ifdef DEBUG
	GlobalVariables::GetInstance()->Update();
#endif // DEBUG
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
					Audio::SoundStopWave(0);
					

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
			startSpriteVelo_ = -4.0f;
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
				}

			}
		}
		postProcess_->SetThreshold(thre_);
		startSpritePos_.y += startSpriteVelo_;
		startEffectSp_->SetPosition(startSpritePos_);
		startEffectSp_->Update();
		break;
	}
	case PLAYGAME: 
	{
		startSpritePos2_.y += 12.0f;
		startEffectSp2_->SetPosition(startSpritePos2_);
		startEffectSp2_->Update();
		enemys_.remove_if([=](std::unique_ptr<Enemy>& bullet) {
			if (bullet->IsDead()) {
				destroyCount_++;
				return true;
			}
			return false;
			});

		items_.remove_if([](std::unique_ptr<PlayerItem>& bullet) {
			if (bullet->IsDead()) {
				return true;
			}
			return false;
			});

		// 現状のクリア条件
		if (destroyCount_ >= 3) {
			IScene::SetSceneNo(CLEAR);
			Audio::SoundStopWave(gameBGM_);
			DeleteObject();
		}
		// 現状のゲームオーバー条件
		// PlayerのHpが０になったら
		if (player_->GetHP() <= 0) {
			IScene::SetSceneNo(GAMEOVER);
			Audio::SoundStopWave(gameBGM_);
			DeleteObject();
		}
		// ロックオン
		lockOn_->Update(enemys_, followCamera_->GetCamera(), player_.get());
		player_->Update();
		followCamera_->Upadate();
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
			(*itr)->Update();
		}

		collisionManager_->CheckAllCollision();
		break;
	}
	}
	
	
	
}
void GameScene::Draw()
{
	
	for (std::list<std::unique_ptr<Enemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Draw(followCamera_->GetCamera());
	}

	for (std::list<std::unique_ptr<WorldDesign>>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Draw(followCamera_->GetCamera());
	}
	for (std::list< std::unique_ptr<PlayerItem>>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Draw(followCamera_->GetCamera());
	}
	ground_->Draw();
	skydome_->Draw(followCamera_->GetCamera());
	player_->Draw(followCamera_->GetCamera());
	switch (gameStateMode_) {
		case PLAYGAME:
			player_->DrawUI();
			break;
	}
	
	lockOn_->Draw();
	
}

void GameScene::Draw2d()
{
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

