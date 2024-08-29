#pragma once
#include <memory>
#include <optional>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"
#include "Character/base/BaseCharacter.h"
#include "json.hpp"
#include "GlobalVariables/GlobalVariables.h"

// ふるまい
enum class Behavior {
	kRoot, // 通常状態
	kAttack, // 攻撃中
	kDash, // ダッシュ中
	kJump
};
class Player : public BaseCharacter
{
public:
	void Init() override;
	void Update() override;
	void Draw(Camera* camera) override;

public:
	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用の媒介変数
		uint32_t dashparam_ = 0;
	};

public:
	void Move();

	
	// 浮遊ギミック初期化
	void InitFloatingGimmmick();
	void UpdateFloatingGimmmick();

	/*---Behavior用関数---*/
	// 攻撃行動
	void BehaviorRootUpdate();
	void BehaviorRootAttackUpdate();
	// 通常行動
	void BehaviorRootInit();
	void BehaviorRootAttackInit();
	// ダッシュ行動
	void BehaviorRootDashInit();
	void BehaviorRootDashUpdate();
	// ジャンプ行動
	void BehaviorRootJumpInit();
	void BehaviorRootJumpUpdate();

	// 調整項目の適用
	void ApplyGlobalVariables();

public:
	const WorldTransform *GetWorldTransform() const  { return &worldTransform_; }
	Behavior GetBehaviorMode() { return behavior_; }

public: // Setter
	void SetCamera(Camera* camera) { camera_ = camera; }
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_body;
	WorldTransform worldTransform_left;
	WorldTransform worldTransform_right;
	WorldTransform worldTransform_head;
	WorldTransform worldTransform_weapon;
	std::unique_ptr<Object3d> object_;
	std::unique_ptr<Object3d> R_arm_;
	std::unique_ptr<Object3d> L_arm_;
	std::unique_ptr<Object3d> head_;
	std::unique_ptr<Object3d> weapon_;

	float angletime = 0.0f;
	float preAngle_ ;
	// 富裕ギミックの媒介変数
	float floatingparam_ = 0.0f;

	// 周期
	const uint16_t period = 120;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * (float)std::numbers::pi * 2.0f / period;
	
	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorkDash workDash_;

	using json = nlohmann::json;

	// 速度
	Vector3 velo_;
};

