#pragma once
#include <memory>
#include <list>

#include "Sprite.h"
enum ScoreDigit {
    ONEDIGIT = 1,
    TWODIGIT = 10,
    THREEDIGIT = 100,
    FOURDIGIT = 1000,
    FIVEDIGIT = 10000,
    SIXDIGIT = 100000
};
class Score
{
public:
	void Init();
	void Update(const int &time, const int& killCount);
	void Draw();
private:
	int gameNumber_[10];
    std::unique_ptr<Sprite> scoreSprite_[6];

    int digitScore_[6];

    int sumScore_;
};

