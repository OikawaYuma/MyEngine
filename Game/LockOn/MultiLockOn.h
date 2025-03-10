/**
* @ file MultiLockOn.h
* @ brief 必殺技の為の攻撃
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include "Sprite.h"
#include "Character/Enemy/Enemy.h"
#include "Character/Player/Player.h"
#include "Camera.h"
class MultiLockOn
{
public:
	void Init();
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* camera, Player* player);
	void Draw();

	const Enemy* GetTarget() { return target_; }

	Vector3 GetTargetPosition() const;

	bool ExistTarget() const { return target_ ? true : false; }

	void Search();

	bool OutRangeJudge(Camera* camera);
private:
	// ロックオンマーク用スプライト
	std::list<std::unique_ptr<Sprite>> lockOnMarks_;
	// ロックオン対象
	const Enemy* target_ = nullptr;
	// 最小距離
	float minDistance_ = 30.0f;
	// 最大距離
	float maxDistance_ = 80.0f;
	// 角度範囲
	float angleRange_ = 0.785f;

private:
	uint32_t lockOnMarkTex_;
};
