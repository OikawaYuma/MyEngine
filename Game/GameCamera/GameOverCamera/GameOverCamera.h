/**
* @ file
* @ brief
*/

#pragma once
#include "Camera.h"
#include <memory>
class GameOverCamera
{
public:
	void Init();
	void Update();
public: // Getter
	Camera* GetCamera() { return camera_.get(); }

private:
	std::unique_ptr<Camera> camera_;
};
