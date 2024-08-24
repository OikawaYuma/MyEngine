#pragma once
#include "Camera.h"
#include "WorldTransform.h"
#include <memory>

class FollowCamera
{
public:
	void Init();
	void Upadate();

public: // Getter
	Camera* GetCamera() { return camera_.get(); }

public:
	void SetTarget(const WorldTransform* target) { target_ = target; }
private:
	std::unique_ptr<Camera> camera_;
	// 追従対象
	const WorldTransform* target_ = nullptr;
};

