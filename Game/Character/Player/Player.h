#pragma once
#include <memory>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"
#include "Character/base/BaseCharacter.h"

class Player : public BaseCharacter
{
public:
	void Init() override;
	void Update() override;
	void Draw(Camera* camera) override;

public:
	void Move();
	// 浮遊ギミック初期化
	void InitFloatingGimmmick();
	void UpdateFloatingGimmmick();
public:
	const WorldTransform *GetWorldTransform() const  { return &worldTransform_; }

public: // Setter
	void SetCamera(Camera* camera) { camera_ = camera; }
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_body;
	WorldTransform worldTransform_left;
	WorldTransform worldTransform_right;
	WorldTransform worldTransform_head;
	std::unique_ptr<Object3d> object_;
	std::unique_ptr<Object3d> R_arm_;
	std::unique_ptr<Object3d> L_arm_;
	std::unique_ptr<Object3d> head_;

	float angletime = 0.0f;

	// 富裕ギミックの媒介変数
	float floatingparam_ = 0.0f;

	// 周期
	const uint16_t period = 120;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * (float)std::numbers::pi * 2.0f / period;
};

