#include "GameScene.h"
#include "Loder.h"
#include "ImGuiCommon.h"
void GameScene::Init()
{
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	player_ = std::make_unique<Player>();
	player_->Init();
	player_->SetCamera(followCamera_->GetCamera());
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransform());

	enemy_ = std::make_unique<Enemy>();
	enemy_->Init();
	enemys_.push_back(enemy_.get());

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Init();
	player_->SetLockOn(lockOn_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Init();
	ground_->SetCamera(followCamera_->GetCamera());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	postProcess_ = new PostProcess();
	postProcess_->Init();
	postProcess_->SetCamera(followCamera_->GetCamera());
	IPostEffectState::SetEffectNo(PostEffectMode::kFullScreen);

}

void GameScene::Update()
{
	GlobalVariables::GetInstance()->Update();
	
	// ロックオン
	lockOn_->Update(enemys_, followCamera_->GetCamera(), player_.get());
	player_->Update();
	followCamera_->Upadate();
	enemy_->Update();
	ground_->Update();
	skydome_->Update();
	
	
	}
void GameScene::Draw()
{
	player_->Draw(followCamera_->GetCamera());
	enemy_->Draw(followCamera_->GetCamera());
	ground_->Draw();
	skydome_->Draw(followCamera_->GetCamera());
	
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

