#pragma once
#include <memory>
#include <list>
#include <thread>
#include <chrono>
#include "Sprite.h"
class GameTimer
{
public:
	void Init();
	void Update();
	void Draw();

	// 計測すr
	void MeasureTime();
private:
	int gameNumber_[10];
	
	int colonTex_ = 0;
	std::unique_ptr<Sprite> colonSprite_;
	// 計測した値
	std::chrono::seconds duration_;
	int gameSecondTime_ = 0;
	std::unique_ptr<Sprite> timeSprite_[4];
};

