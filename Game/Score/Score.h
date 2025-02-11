/**
* @ file Score.h
* @ brief ゲームのスコアに関する設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

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
	void Init(Vector2 pos, Vector2 size, bool isSum,float width);
	void Update(const int &time, const int& killCount);
	void Draw();

public: // Getter
    int32_t GetSumScore() { return sumScore_; }

public: // Setter 
    void SetSumScore(int32_t score) { sumScore_ = score; }
private:
	int gameNumber_[10];
    std::unique_ptr<Sprite> scoreSprite_[6];

    int digitScore_[6];

    int32_t sumScore_;

    // スコアが加算されるか
    bool isSum_ = false;
};

