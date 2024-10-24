/**
* @ file
* @ brief
*/

#pragma once
#include <memory>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"

class Ground
{
public:
	void Init();
	void Update();
	void Draw();
public: // Setter
	void SetCamera(Camera* camera) { camera_ = camera; }
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	std::unique_ptr<Object3d> object_;
	uint32_t skinTex_ = 0;
};

