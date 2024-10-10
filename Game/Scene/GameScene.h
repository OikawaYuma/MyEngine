#pragma once
#include "IScene.h"

#include "PostProcess.h"
#include "CollisionManager.h"	
#include <vector>
#include <memory>
#include "Character/Player/Player.h"
#include "Character/Enemy/Enemy.h"
#include "Ground/Ground.h"
#include "Item/PlayerItem.h"
#include "WorldDesign/WorldDesign.h"
#include "Skydome/Skydome.h"
#include "GameCamera/FollowCamera/FollowCamera.h"
#include "LockOn/LockOn.h"
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
public:
	// 弾リストを取得
	const std::list<std::unique_ptr<Enemy>>& Getbullet() const { return enemys_; }
	const std::list<PlayerItem*>& GetItems() const { return items_; }
	//const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

private:


	

	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<Ground> ground_;
	std::list<PlayerItem*> items_;
	std::list<WorldDesign*> worldDesigns_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<LockOn> lockOn_;
	
	PostProcess* postProcess_ = nullptr;
	uint32_t destroyCount_ = 0;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};

