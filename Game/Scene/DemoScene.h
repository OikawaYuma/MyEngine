#pragma once
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Particle.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "PostProcess.h"
#include "Skybox/Skybox.h"

class DemoScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Draw2d()override;
	void Release()override;
	int GameClose()override;

public:

	void PostEffectChange();

private:
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* demoSprite = nullptr;

	//変数
	uint32_t textureHandle;
	uint32_t textureHandle2;
	uint32_t textureHandle3;
	Object3d* object3d = nullptr;
	Object3d* object3d2 = nullptr;
	Object3d* object3d3 = nullptr;
	Material material;

	WorldTransform worldTransform;
	WorldTransform worldTransform2;
	WorldTransform worldTransform3;

	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Emitter demoEmitter_;
	RandRangePro demoRandPro;
	
	PostProcess* postProcess_ = nullptr;
	std::vector<Object3d*> object3d_;
	Skybox* skybox_ = nullptr;
	Vector3 hsv = {1.0f,1.0f,1.0f};
	float rotateSize_ = 1.057f;


	bool boxTexFlag = true;
};

