/**
* @ file
* @ brief
*/

#pragma once
#include <memory>
#include "IScene.h"
#include "Triangle.h"
/**
* @ file GameScene.h
* @ brief 実際のゲーム部分の関するScene
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
	
	Vector3 hsv = {1.0f,1.0f,1.0f};

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Player> obj_;
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Particle> particle_;
	Emitter emitter_{};
	RandRangePro randRangePro_;

	std::unique_ptr<Object3d> obj2_;
	uint32_t tex_;
	WorldTransform worldtransform_;
	SpotLight spotLight_{};
	Vector3 move_;
	float gravity_;

	float preGravity_;
	float prePower_;
	Vector3 preParticleScale_;

	Material material_{};

	uint32_t gameBGM_;
	uint32_t slimeDeadSE_;
};

