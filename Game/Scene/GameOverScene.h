/**
* @ file GameOverScene.h
* @ brief ゲームオーバー部分の関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "Camera.h"
#include "GameCamera/GameOverCamera/GameOverCamera.h"
#include "Ground/Ground.h"
#include "Skydome/Skydome.h"
#include "Character/Player/Player.h"
#include "Character/Player/GameOver/GameOverPlayer.h"
#include "Character/Enemy/BaseEnemy.h"
#include "Item/PlayerItem.h"
#include "WorldDesign/WorldDesign.h"

enum class BehaviorGameOver {
	kStart,
	kMain,
	kEnd
};

class GameOverScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Draw2d()override;
	void Release()override;
	int GameClose()override;

	void DeleteObject();

private: // Object
	// GameOverシーン用Camera
	std::unique_ptr<GameOverCamera> camera_ = nullptr;
	// GameOverシーン用Player
	std::unique_ptr<GameOverPlayer> player_;
	std::unique_ptr<Player> player2_;
	//　敵
	std::list<std::unique_ptr<BaseEnemy>> enemys_;
	// 地面
	std::unique_ptr<Ground> ground_;
	// Playerの回復アイテム
	std::list<std::unique_ptr<PlayerItem>> items_;
	// 木
	std::list<std::unique_ptr<WorldDesign>> worldDesigns_;
	// 天球
	std::unique_ptr<Skydome> skydome_;
private:
	// ポストエフェクト
	PostProcess* postProcess_ = nullptr;
	float thre_;
	float threPorM_;
	bool threFlag_ = false;
	bool gameOverFlag_ = false;
	// PushUI
	std::unique_ptr<Sprite> pushASp_ = nullptr;
	float pushSpriteAlpha_;
	float pushSpriteAlphaPorM_;
	// ゲーム名のUI
	float gameOverBerYPos = 0.0f;
	float gameOverBerYPosPram = 0.5f;
	std::unique_ptr<Sprite> titleBer_ = nullptr;
	// Texture番号
	uint32_t gameOverColorTex_ = 1;
	uint32_t pushATex_ = 1;
	uint32_t gameOverTex_ = 1;

private: //Audio
	uint32_t gameOverBGM_ = 0;
	uint32_t pushSE_ = 0;
};

