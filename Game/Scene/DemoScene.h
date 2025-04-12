/**
* @ file
* @ brief
*/

#pragma once
#include <memory>
#include "IScene.h"
#include "Triangle.h"
/**
* @ file DemoScene.h
* @ brief 試用の部分に関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/


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
#include "WorldDesign/WorldDesign.h"

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

	void ParticleEmitter();

private:
	
	
	std::unique_ptr<PostProcess> postProcess_;
	// Clearシーン用Camera
	std::unique_ptr<Camera> camera_ = nullptr;
	Vector3 hsv = {1.0f,1.0f,1.0f};

	// 背景オブジェクト
	std::unique_ptr<WorldDesign> wood_;


};

