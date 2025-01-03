/**
* @ file
* @ brief
*/

#pragma once
#include "Camera.h"
#include <memory>
enum CameraNum {
	CAMERA1,
	CAMERA2,
	CAMERA3
};
class TitleCamera
{
public:
	void Init();
	void Update();
public: // Getter
	Camera* GetCamera() { return camera_.get(); }

	uint32_t GetTimer() { return cameraTimer_; }
	uint32_t GetType() { return cameraType_; }
private:
	std::unique_ptr<Camera> camera_;
	uint32_t cameraTimer_ = 0;
	uint32_t cameraType_ = CAMERA1;
	
};

