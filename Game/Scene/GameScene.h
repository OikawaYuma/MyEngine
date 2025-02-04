/**
* @ file GameScene.h
* @ brief 実際のゲーム部分の関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "IScene.h"

#include "PostProcess.h"
#include "CollisionManager.h"	
#include <vector>
#include <memory>
#include "Character/Player/Player.h"
#include "Character/Enemy/BaseEnemy.h"
#include "Character/Enemy/EnemyApear.h"
#include "Ground/Ground.h"
#include "Item/PlayerItem.h"
#include "WorldDesign/WorldDesign.h"
#include "Skydome/Skydome.h"
#include "GameCamera/FollowCamera/FollowCamera.h"
#include "LockOn/LockOn.h"
#include "Camera.h"
#include "Loading/GameLoading.h"
#include "Score/Score.h"
#include "GameTimer/GameTimer.h"
/*--------------------------------------------
ゲームのスタート演出のための待ちを管理するためのenum
---------------------------------------------*/
enum GameState {
	WAITGAME,
	STARTGAME,
	PLAYGAME,
	CLEARGAME,
	DEADGAME,
	POSEMENUGAME,
	TITLEGAME,
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
	// 弾リストを取得
	const std::list<std::unique_ptr<BaseEnemy>>& Getbullet() const { return enemys_; }
	const std::list<std::unique_ptr<PlayerItem>>& GetItems() const { return items_; }

private: //Object
	// プレイヤー
	std::unique_ptr<Player> player_;
	// エネミーリスト
	std::list<std::unique_ptr<BaseEnemy>> enemys_;
	// 床
	std::unique_ptr<Ground> ground_;
	std::list<std::unique_ptr<PlayerItem>> items_;
	// 背景オブジェクト（木）
	std::list<std::unique_ptr<WorldDesign>> worldDesigns_;
	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// シングルロックオン
	std::unique_ptr<LockOn> lockOn_;
	// スコア
	std::unique_ptr<Score> score_;
	// 敵を倒した数
	int killCount_ = 0;
	// ゲーム用タイマー
	std::unique_ptr<GameTimer> gameTimer_;

private: //Collision
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

private:
	// ポストエフェクト
	PostProcess* postProcess_ = nullptr;
	// ゲームの進行度
	uint32_t gameStateMode_ = PLAYGAME;
	float thre_ = 0.0f;
	float threPorM_ = 0.025f;
	bool threFlag_ = false;
	uint32_t destroyCount_ = 0;

private: // threadを使用した際に使うようLoadingUI
	// 
	std::unique_ptr<GameLoading> loading_;
	Vector2 startSpritePos_;
	float startSpriteVelo_;
	std::unique_ptr<Sprite> startEffectSp_;
	uint32_t startEffectSpTex_ = 0;
	

private: // ゲーム開始までの演出
	bool cameraFlag_ = false;
	Vector2 startSpritePos2_;
	float startSpriteVelo2_;
	std::unique_ptr<Sprite> startEffectSp2_;
	uint32_t startEffectSpTex2_ = 0;
	uint32_t startTimer_ = 0;
	bool moveflag1 = false;
	bool moveFlag2 = false;
private:
	SpotLight spotLight_{};
	float cosAngle_ = 0.0f;
	
	
	// クリア条件
	int clearFlagCount_ = 0;
	EnemyApear enemyApear_;

public: //posemenu
	std::unique_ptr<Sprite> titlepose_;
	std::unique_ptr<Sprite> titlepose2_;

	uint32_t titleposeTex1_;
	uint32_t titleposeTex2_;
	bool titleposeFlag1_;

	bool titleposeFlag2_;

private: // Audio
	uint32_t gameBGM_;
	uint32_t slimeDeadSE_;
	uint32_t gameClearSE_;
	

};

