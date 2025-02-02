/**
* @ file LockOn.h
* @ brief ロックオンの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include "Sprite.h"
#include "Character/Enemy/BaseEnemy.h"
#include "Character/Player/Player.h"
#include "Camera.h"
class LockOn
{
public:
	void Init();
	void Update(const std::list<std::unique_ptr<BaseEnemy>>& enemies,Camera* camera,Player * player);
	void Draw();

	const BaseEnemy* GetTarget() { return target_; }

	Vector3 GetTargetPosition() const;

	bool ExistTarget() const { return target_ ? true : false; }

	void Search();

	bool OutRangeJudge(Camera* camera);
private:
	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;
	// ロックオン対象
	const BaseEnemy* target_ = nullptr;
	// 最小距離
	float minDistance_ = 30.0f;
	// 最大距離
	float maxDistance_ = 80.0f;
	// 角度範囲
	float angleRange_ = 0.785f;

private:
	uint32_t lockOnMarkTex_;
};

