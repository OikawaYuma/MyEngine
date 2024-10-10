#pragma once
#include <memory>
#include "Sprite.h"
#include "Character/Enemy/Enemy.h"
#include "Character/Player/Player.h"
#include "Camera.h"
class LockOn
{
public:
	void Init();
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies,Camera* camera,Player * player);
	void Draw();

	const Enemy* GetTarget() { return target_; }

	Vector3 GetTargetPosition() const;

	bool ExistTarget() const { return target_ ? true : false; }

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

