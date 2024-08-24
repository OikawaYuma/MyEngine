#pragma once
#include <memory>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"

class Skydome
{
public:
	void Init();
	void Update();
	void Draw(Camera* camera);

public: // Setter
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	std::unique_ptr<Object3d> object_;
	uint32_t skinTex_;
};

