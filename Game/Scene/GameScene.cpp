#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
#include "Object3dManager.h"
#include <Audio.h>
void GameScene::Init()
{
	
	Object3dManager::GetInstance()->Init();
	DeleteObject();
	// Camera初期化
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();
	// Player初期化
	player_ = std::make_unique<Player>();
	player_->Init({0.0f,1.0f,0.0f,},"player");
	player_->SetCamera(followCamera_->GetCamera());
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransform());
	// Playerの位置を取得した後のカメラ調整
	followCamera_->PosAdustment();
	// Skydome初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();
	// Ground初期化
	ground_ = std::make_unique<Ground>();
	ground_->SetCamera(followCamera_->GetCamera());

	// Loderによる読み込み
	Loder::LoadJsonFile("Resources/json", "stage6", player_.get(), enemys_, items_, worldDesigns_, ground_.get());
	
	// LockOn初期化
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Init();
	// PlayerとCameraにLockOnの値を渡す
	player_->SetLockOn(lockOn_.get());
	followCamera_->SetLockOn(lockOn_.get());
	// スポットライト初期化
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
	// Gameの演出モード
	gameStateMode_ = WAITGAME;
	// PostEffect初期化
	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	postProcess_->SetDissolveInfo({1.0f,1.0f,1.0f});
	postProcess_->SetEffectNo(PostEffectMode::kDissolveOutline);
	postProcess_->SerDissolveOutline({ .weightSize = 100.0f });
	thre_ = 1.0f;
	threPorM_ = 0.025f;
	threFlag_ = false;
	postProcess_->SetThreshold(thre_);
	// CollisionManager初期化
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->SetGameScene(this);
	collisionManager_->SetPlayer(player_.get());
	// 敵を倒した数
	destroyCount_ = 0;
	cameraFlag_ = false;
	// StartEffectUI
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

	// Loading
	loading_ = std::make_unique<GameLoading>();
	loading_->Init(JUMPONE);

	// Audio
	gameBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/game.wav");
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

	// Enemyのポップ初期化
	enemyApear_.Init();
	enemyApear_.SetPlayer(player_.get());
	
	Object3dManager::ObjectSort();
	Enemy::SetEnemydestory(0);
}

void GameScene::Update()
{
	
#ifdef DEBUG
	GlobalVariables::GetInstance()->Update();
#endif // DEBUG
	DissolveOutlineInfo dissolveOutlineInfo = postProcess_->GetDissolveOutline();
	
	postProcess_->SerDissolveOutline({.projectionInverse = Inverse(followCamera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = {1.0f,1.0f,1.0f} , .weightSize  = dissolveOutlineInfo.weightSize});
	spotLight_.cosAngle =
		std::cos(std::numbers::pi_v<float> / cosAngle_);
	spotLight_.direction = Normalize(spotLight_.direction);
	switch (gameStateMode_) {
		// ロード画面
	case WAITGAME:
	{
		loading_->Update();
		// Loading演出が終わった時にSTARTGAMEに移行する
		if (loading_->GetLoadingEffectEnd()) {
			thre_ -= threPorM_;;
			if (thre_ <= 0.5f) {
				followCamera_->StartCameraEffect();
			}
			// ゲーム画面に移る
			if (thre_ <= 0.0f) {
				gameStateMode_ = STARTGAME;
				threPorM_ = 0.025f;
				threFlag_ = true;
				Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), gameBGM_, true);
			}
			postProcess_->SetThreshold(thre_);
		}
	
		
		break;
	}
	// ゲーム
	case STARTGAME: {
		if (!cameraFlag_) {
			followCamera_->StartCameraEffect();
		}
		
		startTimer_++;
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
			
			if (startTimer_ >= 300) {
				moveFlag2 = true;
			}
		}
		else if (moveFlag2) {
			startSpriteVelo_ = -5.0f;
		}

		if (startTimer_ >=360.0f) {
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

		// 現状のクリア条件
		if (gameTimer_->GetGameTime() <= 0) {

			Audio::SoundStopWave(gameBGM_);
			postProcess_->SetDissolveInfo({ 1.0f, 0.984313f, 0.643f });
			postProcess_->SerDissolveOutline({ .projectionInverse = Inverse(followCamera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = { 1.0f, 0.984313f, 0.643f } , .weightSize = dissolveOutlineInfo.weightSize });
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
			postProcess_->SerDissolveOutline({ .projectionInverse = Inverse(followCamera_->GetCamera()->GetProjectionMatrix()),.threshold = thre_,.discardColor = { 0.35f, 0.025f, 0.025f }  , .weightSize = dissolveOutlineInfo.weightSize });
			
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
		for (std::list<std::unique_ptr<BaseEnemy>>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
			(*itr)->Update();

			
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
			IScene::SetSceneNo(CLEAR);
		}

		postProcess_->SetThreshold(thre_);
		break;
	}
	case DEADGAME:
	{
		thre_ -= threPorM_;;
		if (thre_ >= 1.2f) {
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
	loading_->Draw();
	startEffectSp_->Draw(startEffectSpTex_, { 1.0f,1.0f,1.0f,1.0f });
	startEffectSp2_->Draw(startEffectSpTex2_, { 1.0f,1.0f,1.0f,1.0f });
}

void GameScene::PostDraw()
{
	
	postProcess_->Draw();
	
}

void GameScene::Release() {
	postProcess_->Release();
	delete postProcess_;
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

