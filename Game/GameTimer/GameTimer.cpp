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
}

void GameTimer::Update()
{
	for (int i = 0; i < 4; i++) {
		timeSprite_[i]->Update();
	}
	std::thread worker_(MeasureTime);
	worker_.join();

	gameSecondTime_ += duration_.count();
	
	colonSprite_->Update();
}

void GameTimer::Draw()
{
	for (int i = 0; i < 4; i++) {
		timeSprite_[i]->Draw(gameNumber_[], { 1.0f,1.0f,1.0f,1.0f });
	}
	colonSprite_->Draw(colonTex_, { 1.0f,1.0f,1.0f,1.0f });
}

void GameTimer::MeasureTime()
{
	// 時間の開始
	auto start = std::chrono::high_resolution_clock::now();

	// 処理をシミュレート (例えば、1秒待機)
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// 時間の終了
	auto end = std::chrono::high_resolution_clock::now();

	// 時間差を計算
	duration_ = std::chrono::duration_cast<std::chrono::seconds>(end - start);

	
}
