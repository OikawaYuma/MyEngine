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

	void SetSpotLight(const SpotLight& spotLight) { spotLight_ = spotLight; }
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	std::unique_ptr<Object3d> object_;
	Material material_{};
	DirectionalLight direLight_{};
	SpotLight spotLight_{};
	uint32_t skinTex_ = 0;
};

