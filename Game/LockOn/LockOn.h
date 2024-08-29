#pragma once
#include <memory>
#include "Sprite.h"
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Camera.h"
class LockOn
{
public:
	void Init();
	void Update(const std::list<Enemy*>& enemies,Camera* camera,Player * player);
	void Draw();

	const Enemy* GetTarget() { return target_; }

	void Search();

	bool OutRangeJudge(Camera* camera);
private:
	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;
	// ロックオン対象
	const Enemy* target_ = nullptr;
	// 最小距離
	float minDistance_ = 10.0f;
	// 最大距離
	float maxDistance_ = 50.0f;
	// 角度範囲
	float angleRange_ = 1.57f;

private:
	uint32_t lockOnMarkTex_;
};

