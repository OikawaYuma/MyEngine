#include "Slime2d.h"

void Slime2d::Init(Vector2 pos, float jumpPower, uint32_t jumpWaitTimer, bool isJump)
{
	slime2DSp_ = std::make_unique<Sprite>();
	pos_ = pos;
	slime2DSp_->Init(
		pos , { 100, 1000 },
		{ 0.5f,0.5f }, { 1.0f,1.0f,1.0,1.0 },
		"Resources/noise1.png");
	jumpPower_ = (float)jumpWaitTimer;
	gravity_ = (float)jumpWaitTimer;
	jumpWaitTimer_ = jumpPower;
	isjump_ = isJump;
	startFlag = false;
	apearVelo_ = 4;
	apearFlag_ = true;
	slime2DSpTex_ = TextureManager::StoreTexture("Resources/slimeSprite.png");
}

void Slime2d::StartUpdate()
{
	pos_.y -= apearVelo_;
	if (pos_.y <= 630.0f && apearFlag_) {
		apearVelo_ *= -1;
		apearFlag_ = false;
	}
	if (pos_.y >= 650.0f && !apearFlag_) {
		pos_.y = 650.0f;
		startFlag =true;
	}
	slime2DSp_->SetPosition(pos_);
	slime2DSp_->Update();
}

void Slime2d::EndUpdate()
{
	pos_.y -= apearVelo_;
	if (pos_.y <= 630.0f && apearFlag_) {
		apearVelo_ *= -1;
		apearFlag_ = false;
	}
	if (pos_.y >= 800.0f && !apearFlag_) {
		pos_.y = 800.0f;
		startFlag = true;
	}
	slime2DSp_->SetPosition(pos_);
	slime2DSp_->Update();
}

void Slime2d::Update()
{
	

		slimeTimer_ += 1;
		
		pos_.y -= jumpPower_;
		jumpPower_ -= jumpWaitTimer_;
		if (pos_.y > 650.0f) {
			pos_.y = 650.0f;
			slimeTimer_ = 0;
			jumpPower_ = gravity_;
			isjump_ = false;
		}
		

		slime2DSp_->SetPosition(pos_);
		slime2DSp_->Update();
	
	
}

void Slime2d::Draw()
{
	slime2DSp_->Draw(slime2DSpTex_, { 1.0f,1.0f,1.0f,1.0f });
}
