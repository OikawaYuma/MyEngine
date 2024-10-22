#pragma once
#include "IScene.h"

#include "PostProcess.h"
#include "CollisionManager.h"	
#include <vector>




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


	

	PostProcess* postProcess_ = nullptr;
	uint32_t destroyCount_ = 0;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	
};

