#include "GameTimer.h"
#include "TextureManager.h"
#include <iostream>
#include <chrono>

void GameTimer::Init()
{
	for (int i = 0; i < 10; i++) {
		gameNumber_[i] = TextureManager::GetInstance()->StoreTexture("Resources/number/number" + std::to_string(i) + ".png");
	}
	colonTex_ = TextureManager::GetInstance()->StoreTexture("Resources/number/colon.png");
	for (int i = 0; i < 2; i++) {
		timeSprite_[i] = std::make_unique<Sprite>();
		timeSprite_[i]->Init(
			{ 640 + i * 36.0f - 72.0f,32.0f },
			{ 64.0, 64.0f },
			{ 0.5f , 0.5f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			"Resources/Reticle.png");
	}
	for (int i = 2; i < 4; i++) {
		timeSprite_[i] = std::make_unique<Sprite>();
		timeSprite_[i]->Init(
			{ 640 + i * 36.0f - 36.0f,32.0f },
			{ 64.0, 64.0f },
			{ 0.5f , 0.5f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			"Resources/Reticle.png");
	}
	colonSprite_ = std::make_unique<Sprite>();
	colonSprite_->Init(
		{ 640.0f ,32.0f },
		{ 64.0, 64.0f },
		{ 0.5f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		"Resources/Reticle.png");
	gameTime_.sumTime = 60;
	frame_ = 0;
	apearFlag_ = false;
}

void GameTimer::Start()
{
	// 時間の開始
	start_ = std::chrono::steady_clock::now();
}

void GameTimer::Update(const float& apearPoint)
{
	for (int i = 0; i < 4; i++) {
		timeSprite_[i]->Update();
	}
	MeasureTime();

	gameTime_.minutesTwoDigit = gameTime_.sumTime % 3600 /60 /10;
	gameTime_.minutesOneDigit = gameTime_.sumTime % 3600 / 60 % 10; //1250 % 3600  = 0 + 1250 /60 = 27
	gameTime_.secondsTwoDigit = gameTime_.sumTime % 3600 % 60 /10;//141 %3600  = 0 + 141 /60 = 2 + 21 / 10 = 1
	gameTime_.secondsOneDigit = gameTime_.sumTime % 3600 % 60 % 10;//141 %3600  = 0 + 141 /60 = 2 + 21 % 10 = 1
	colonSprite_->Update();

	if (apearPoint >=32.0f) {
		apearFlag_ = true;
	}
}

void GameTimer::Draw()
{
	if (apearFlag_) {
		timeSprite_[0]->Draw(gameNumber_[gameTime_.minutesTwoDigit], { 1.0f,1.0f,1.0f,1.0f });
		timeSprite_[1]->Draw(gameNumber_[gameTime_.minutesOneDigit], { 1.0f,1.0f,1.0f,1.0f });
		timeSprite_[2]->Draw(gameNumber_[gameTime_.secondsTwoDigit], { 1.0f,1.0f,1.0f,1.0f });
		timeSprite_[3]->Draw(gameNumber_[gameTime_.secondsOneDigit], { 1.0f,1.0f,1.0f,1.0f });

		colonSprite_->Draw(colonTex_, { 1.0f,1.0f,1.0f,1.0f });
	}
}

void GameTimer::MeasureTime()
{
	
	frame_ += 1;
	if (gameTime_.sumTime > 0) {
		if (frame_ >= 40) {
			gameTime_.sumTime -= 1;
			frame_ = 0;
		}
	}

	//// 時間の終了
	//auto end = std::chrono::steady_clock::now();

	//// 時間差を計算
	//gameTime_.milliSeconds += static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count());

	//gameTime_.sumTime = gameTime_.milliSeconds / 1000;

	
}
