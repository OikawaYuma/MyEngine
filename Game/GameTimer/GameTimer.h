/**
* @ file GameTimer.h
* @ brief ゲームのプレイ時間に関するクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/
#pragma once
#include <memory>
#include <list>
#include <thread>
#include <chrono>
#include "Sprite.h"

// 時間を時分秒で桁ごとに管理するための構造たxが1桁目、yが2桁目を表す
struct ClockTime {
	int sumTime;
	int milliSeconds;
	int minutesOneDigit;
	int minutesTwoDigit;
	int secondsOneDigit;
	int secondsTwoDigit;
};

class GameTimer
{
public:
	void Init();
	void Start();
	void Update(const float &apearPoint);
	void Draw();

	// 計測すr
	void MeasureTime();

public://Accecer
	int GetGameTime() { return gameTime_.sumTime; }

private:
	int gameNumber_[10];
	int frame_ = 0;
	int colonTex_ = 0;
	std::unique_ptr<Sprite> colonSprite_;
	// 計測した値
	std::chrono::seconds duration_;

	std::chrono::steady_clock::time_point start_;
	ClockTime gameTime_;

	std::unique_ptr<Sprite> timeSprite_[4];

	bool apearFlag_ = false;
};

