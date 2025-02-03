/**
* @ file Loading.h
* @ brief threadを使用した際のLoading画面のクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Slime2d/Slime2d.h"

class GameLoading
{
public:
	void Init(uint32_t jumpMode);
	void Update();
	void Draw();

public: // Getter
	bool GetLoadingEffectEnd() { return loadingEffectEnd_; };

private:
	std::unique_ptr<Sprite> LoadStringSp_;
	uint32_t LoadStringSpTex_ = 0;
	std::unique_ptr<Slime2d> slime2DSp1_;
	std::unique_ptr<Slime2d> slime2DSp2_;
	std::unique_ptr<Slime2d> slime2DSp3_;

	bool moveflag1 = false;
	bool moveFlag2 = false;
	uint32_t jumpMode_ = JUMPONE;
	uint32_t jumpRoopNum = 0;
	float loadpos = 0;
	uint32_t endTimer = 0;
	uint32_t startTimer = 0;
	// Loading演出の終了を管理する変数
	bool loadingEffectEnd_ = false;
};

