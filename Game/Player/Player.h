#pragma once
#include <memory>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"

class Player
{
public:
	void Init();
	void Update();
	void Draw();

public:
	void Move();
public:
	const WorldTransform *GetWorldTransform() const  { return &worldTransform_; }

public: // Setter
	void SetCamera(Camera* camera) { camera_ = camera; }
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	WorldTransform worldTransform_2;
	std::unique_ptr<Object3d> object_;
	std::unique_ptr<Object3d> R_arm_;
	std::unique_ptr<Object3d> L_arm_;
	std::unique_ptr<Object3d> head_;
	uint32_t skinTex_;

	float angletime = 0.0f;
};

