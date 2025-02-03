#include "GameLoading.h"

void GameLoading::Init(uint32_t jumpMode)
{
	jumpMode_ = jumpMode;
	switch (jumpMode_)
	{
	
	case JUMPONE: {
		// LoadingUI
		LoadStringSp_ = std::make_unique<Sprite>();
		LoadStringSp_->Init(
			{ 800,660 }, { 500, 120 },
			{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
			"Resources/noise1.png");
		LoadStringSpTex_ = TextureManager::StoreTexture("Resources/LoadString.png");
		moveflag1 = false;
		moveFlag2 = false;
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
		startTimer = 0;
		break;
	}
	case JUMPSTART: {
		LoadStringSp_ = std::make_unique<Sprite>();
		loadpos = 800;
		LoadStringSp_->Init(
			{ 800,loadpos }, { 500, 120 },
			{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
			"Resources/noise1.png");
		LoadStringSpTex_ = TextureManager::StoreTexture("Resources/LoadString.png");

		slime2DSp1_ = std::make_unique<Slime2d>();
		slime2DSp1_->Init(
			{ 1050,800 }, 0.2f, 2, true);

		slime2DSp2_ = std::make_unique<Slime2d>();
		slime2DSp2_->Init(
			{ 1135,800 }, 0.2f, 3, false);

		slime2DSp3_ = std::make_unique<Slime2d>();
		slime2DSp3_->Init(
			{ 1220,800 }, 0.25f, 4, false);

		break;
	}
	}
	

	// Loading演出の終了を管理するフラグ
	loadingEffectEnd_ = false;
}

void GameLoading::Update()
{
	switch (jumpMode_) {
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
				loadingEffectEnd_ = true;
			}
		}


		LoadStringSp_->SetPosition({ 800.0f,loadpos });
		LoadStringSp_->Update();

		break;
	}
	case JUMPONE: {
		startTimer++;
		if (startTimer >= 10.0f) {
			slime2DSp1_->Update();
			if (!slime2DSp1_->GetIsJump()) {
				jumpMode_ = JUMPTWO;
				slime2DSp2_->SetIsJump(true);
			}
		}
		break;
	}
	case JUMPTWO: {
		slime2DSp2_->Update();
		if (!slime2DSp2_->GetIsJump()) {
			jumpMode_ = JUMPTHREE;
			slime2DSp3_->SetIsJump(true);
		}
		break;
	}
	case JUMPTHREE: {
		slime2DSp3_->Update();
		if (!slime2DSp3_->GetIsJump()) {
			jumpMode_ = JUMPONE;
			slime2DSp1_->SetIsJump(true);
			jumpRoopNum++;
			if (jumpRoopNum >= 2) {
				jumpMode_ = JUMPEND;
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

				// Loading終了
				loadingEffectEnd_ = true;
			}
		}

		break;
	}
	}
}

void GameLoading::Draw()
{
	LoadStringSp_->Draw(LoadStringSpTex_, { 1.0f,1.0f,1.0f,1.0f });
	slime2DSp1_->Draw();
	slime2DSp2_->Draw();
	slime2DSp3_->Draw();
}
