/**
* @ file
* @ brief
*/

#pragma once
#include <memory>
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
#include "Ground/Ground.h"
#include "Character/Player/Player.h"


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
	
	
	PostProcess* postProcess_ = nullptr;
	
	Vector3 hsv = {1.0f,1.0f,1.0f};

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Player> obj_;
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Object3d> obj2_;
	uint32_t tex_;
	WorldTransform worldtransform_;
	SpotLight spotLight_{};

	
};

