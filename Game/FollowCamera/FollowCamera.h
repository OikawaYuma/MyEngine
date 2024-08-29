#pragma once
#include "Camera.h"
#include "WorldTransform.h"
#include <memory>

class FollowCamera
{
public:
	void Init();
	void Upadate();

	void Reset();
	

public: // Getter
	Camera* GetCamera() { return camera_.get(); }

public:
	void SetTarget(const WorldTransform* target);
private:
	std::unique_ptr<Camera> camera_;
	// 追従対象
	const WorldTransform* target_ = nullptr;

	float destinationAngleY_;
	// 追従対象の残像座標
	Vector3 interarget_ = {};

	float cameraTime_ = 1.0f;
	bool isDash_ = false;
	float angletime;

	bool resetAngleFlag = false;
	float resetAngleTime_ = 0.0f;
};

