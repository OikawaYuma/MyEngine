#pragma once
#include "IScene.h"

#include "PostProcess.h"
#include "CollisionManager.h"	
#include <vector>
#include <memory>
#include "Character/Player/Player.h"
#include "Character/Enemy/Enemy.h"
#include "Ground/Ground.h"
#include "Skydome/Skydome.h"
#include "FollowCamera/FollowCamera.h"
#include "Camera.h"





class GameScene :public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Draw2d()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;
public:
	

private:


	

	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<FollowCamera> followCamera_;
	PostProcess* postProcess_ = nullptr;
	uint32_t destroyCount_ = 0;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};

