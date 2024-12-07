/**
* @ file
* @ brief
*/

#pragma once
#include "Object3d.h"
#include <list>
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "PlaneProjectionShadow.h"

class WorldDesign
{
public:
	void Init(const Vector3& scale, const Vector3& translate, const std::string filename);
	void Update();
	void Draw(Camera* camera);
public: // Getter
	Object3d* GetWorldDesignObject() { return object_.get(); };

	void SetSpotLight(const SpotLight& spotLight) { spotLight_ = spotLight; }
private:
	std::unique_ptr<Object3d> object_;
	std::unique_ptr<PlaneProjectionShadow> shadowObject_;
	uint32_t floorTex_ = 0;
	WorldTransform worldTransform_;
	Camera* camera_ = nullptr;

	Vector4 color_;

	Material material_{};
	DirectionalLight direLight_{};
	SpotLight spotLight_{};
};

