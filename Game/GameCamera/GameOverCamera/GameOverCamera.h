/**
* @ file GameOverCamera.h
* @ brief GameOverSceneのカメラ設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
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

