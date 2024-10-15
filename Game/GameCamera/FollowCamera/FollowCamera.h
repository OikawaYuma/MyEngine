#pragma once
#include "Camera.h"
#include "WorldTransform.h"
#include <memory>

// 
class LockOn;
class FollowCamera
{
public:
	void Init();
	void Upadate();
	void Reset();

	// プレイヤーの位置調整
	void PosAdustment();
	

public: // Getter
	Camera* GetCamera() { return camera_.get(); }

public:
	void SetTarget(const WorldTransform* target);
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn;};
private:
	std::unique_ptr<Camera> camera_;
	// 追従対象
	const WorldTransform* target_ = nullptr;
	// ロックオン
	LockOn* lockOn_ = nullptr;

	float destinationAngleY_ = 0.0f;
	// 追従対象の残像座標
	Vector3 interarget_ = {};

	float cameraTime_ = 1.0f;
	bool isDash_ = false;
	float angletime = 0.0f;

	bool resetAngleFlag = false;
	float resetAngleTime_ = 0.0f;
};

