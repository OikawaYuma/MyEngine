/**
* @ file
* @ brief
*/

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
#include "Slime2d/Slime2d.h"
/*--------------------------------------------
ゲームのスタート演出のための待ちを管理するための構造体
---------------------------------------------*/
enum GameState {
	WAITGAME,
	STARTGAME,
	PLAYGAME,
	CLEARGAME,
	DEADGAME
};


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
	void DeleteObject();
public:
public:
	// 弾リストを取得
	const std::list<std::unique_ptr<Enemy>>& Getbullet() const { return enemys_; }
	const std::list<std::unique_ptr<PlayerItem>>& GetItems() const { return items_; }
	//const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

private:


	

	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<Ground> ground_;
	std::list<std::unique_ptr<PlayerItem>> items_;
	std::list<std::unique_ptr<WorldDesign>> worldDesigns_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<LockOn> lockOn_;
	
	PostProcess* postProcess_ = nullptr;
	uint32_t gameStateMode_ = PLAYGAME;
	float thre_ = 0.0f;
	float threPorM_ = 0.025f;
	bool threFlag_ = false;
	uint32_t destroyCount_ = 0;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;


	std::unique_ptr<Sprite> LoadStringSp_;
	uint32_t LoadStringSpTex_ = 0;

	std::unique_ptr<Slime2d> slime2DSp1_;
	std::unique_ptr<Slime2d> slime2DSp2_;
	std::unique_ptr<Slime2d> slime2DSp3_;


	Vector2 startSpritePos_;
	float startSpriteVelo_;
	std::unique_ptr<Sprite> startEffectSp_;
	uint32_t startEffectSpTex_ = 0;
	bool moveflag1 = false;
	bool moveFlag2 = false;
	uint32_t statrTimer_ = 0;
	bool cameraFlag_ = false;

	Vector2 startSpritePos2_;
	float startSpriteVelo2_;
	std::unique_ptr<Sprite> startEffectSp2_;
	uint32_t startEffectSpTex2_ = 0;

	SpotLight spotLight_{};
	float cosAngle_ = 0.0f;
	uint32_t jumpNum_ = JUMPONE;
	uint32_t jumpRoopNum = 0;
	float loadpos = 0;
	uint32_t endTimer = 0;
	uint32_t startTimer = 0;
	uint32_t gameBGM_;

	uint32_t slimeDeadSE_;
	uint32_t gameClearSE_;
	bool GameOverFlag;

	// クリア条件
	int clearFlagCount_ = 0;
};

