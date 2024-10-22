#pragma once
#include "Vector2.h"
#include <memory>
#include "Sprite.h"
enum jumpNum {
	JUMPONE,
	JUMPTWO,
	JUMPTHREE,
	JUMPSTART,
	JUMPEND
};
class Slime2d
{
public:
	void Init(Vector2 pos, float jumpPower, uint32_t jumpWaitTimer, bool isJump);
	void StartUpdate();
	void EndUpdate();
	void Update();
	void Draw();

	bool GetIsJump() { return isjump_; }
	void SetIsJump(bool isjump) { isjump_ = isjump; }
	bool IsApear() { return apearFlag_; }
	bool IsStart() { return startFlag; }
private:
	std::unique_ptr<Sprite> slime2DSp_;
	uint32_t slime2DSpTex_ = 0;
	Vector2 pos_{};
	float jumpPower_ = 0.0f;
	float gravity_ = 0;
	float jumpWaitTimer_ = 0;
	uint32_t slimeTimer_ = 0;
	bool isjump_ = false;

	float apearVelo_ = 0;
	bool apearFlag_ = false;

	bool startFlag = false;


};

