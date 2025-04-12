/**
* @ file WorldDesign.h
* @ brief 背景Objectの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
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
	~WorldDesign();
	void Init(const Vector3& scale, const Vector3& translate, const std::string filename);
	void Update();
public: // Getter


	void SetSpotLight(const SpotLight& spotLight) { spotLight_ = spotLight; }

	void SetObjectPram();
private:
	std::unique_ptr<PlaneProjectionShadow> shadowObject_;
	std::weak_ptr<ObjectPram> objectPram_{};
	std::weak_ptr<ObjectPram> shadowObjectPram_{};
	uint32_t floorTex_ = 0;
	Camera* camera_ = nullptr;

	Vector4 color_;

	Material material_{};
	DirectionalLight direLight_{};
	SpotLight spotLight_{};
};

