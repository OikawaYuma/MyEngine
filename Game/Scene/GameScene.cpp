#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
void GameScene::Init()
{
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	player_ = std::make_unique<Player>();
	player_->SetCamera(followCamera_->GetCamera());
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransform());
	Loder::LoadJsonFile("Resources/json", "stage", player_.get(), ground_.get(), enemys_, items_, worldDesigns_);
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Init();
	player_->SetLockOn(lockOn_.get());
	followCamera_->SetLockOn(lockOn_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Init();
	ground_->SetCamera(followCamera_->GetCamera());
	
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	IPostEffectState::SetEffectNo(PostEffectMode::kFullScreen);
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->SetGameScene(this);
	collisionManager_->SetPlayer(player_.get());
	destroyCount_ = 0;
}

void GameScene::Update()
{
	
	GlobalVariables::GetInstance()->Update();
	enemys_.remove_if([=](Enemy* bullet) {
		if (bullet->IsDead()) {
			destroyCount_++;
			delete bullet;
			return true;
		}
		return false;
		});

	items_.remove_if([](PlayerItem* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	// 現状のクリア条件
	if (destroyCount_ >= 3) {
		IScene::SetSceneNo(CLEAR);
	}
	// ロックオン
	lockOn_->Update(enemys_, followCamera_->GetCamera(), player_.get());
	player_->Update();
	followCamera_->Upadate();
	ground_->Update();
	skydome_->Update();
	// エネミーの弾発射処理
	for (std::list<Enemy*>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Update();
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
	}
	for (std::list<PlayerItem*>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Update();
	}
	

	for (std::list<WorldDesign*>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Update();
	}

	collisionManager_->CheckAllCollision();
	
}
void GameScene::Draw()
{
	
	for (std::list<Enemy*>::iterator itr = enemys_.begin(); itr != enemys_.end(); itr++) {
		(*itr)->Draw(followCamera_->GetCamera());
	}

	for (std::list<WorldDesign*>::iterator itr = worldDesigns_.begin(); itr != worldDesigns_.end(); itr++) {
		(*itr)->Draw(followCamera_->GetCamera());
	}
	for (std::list<PlayerItem*>::iterator itr = items_.begin(); itr != items_.end(); itr++) {
		(*itr)->Draw(followCamera_->GetCamera());
	}
	ground_->Draw();
	skydome_->Draw(followCamera_->GetCamera());
	player_->Draw(followCamera_->GetCamera());
	
}

void GameScene::Draw2d()
{
	player_->DrawUI();
	lockOn_->Draw();
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

