#pragma once
#include "Object3d.h"
#include <list>
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"

class WorldDesign
{
public:
	void Init(const Vector3& scale, const Vector3& translate, const std::string filename);
	void Update();
	void Draw(Camera* camera);
public: // Getter
	Object3d* GetWorldDesignObject(){ return object_.get(); }
private:
	std::unique_ptr<Object3d> object_;
	uint32_t floorTex_;
	WorldTransform worldTransform_;
	Camera* camera_ = nullptr;
};

