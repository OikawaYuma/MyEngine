/**
* @ file ClearScene.h
* @ brief ゲームのクリア部分に関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/


#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "Camera.h"
#include "GameCamera/GameOverCamera/GameOverCamera.h"
#include "GameCamera/ClearCamera/ClearCamera.h"
#include "Ground/Ground.h"
#include "Skydome/Skydome.h"
#include "Character/Player/Player.h"
#include "Character/Player/Clear/ClearPlayer.h"
#include "Character/Enemy/BaseEnemy.h"
#include "Item/PlayerItem.h"
#include "WorldDesign/WorldDesign.h"
#include "Score/Score.h"
enum class BehaviorGameClear {
	kStart,
	kMain,
	kEnd
};

class ClearScene : public IScene
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

private: // Object
	// Clearシーン用Camera
	std::unique_ptr<ClearCamera> camera_ = nullptr;
	// Clearシーン用Player
	std::unique_ptr<ClearPlayer> player_;
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
	bool clearFlag_ = false;
	// PushUI
	std::unique_ptr<Sprite> pushASp_ = nullptr;
	float pushSpriteAlpha_;
	float pushSpriteAlphaPorM_;
	// ゲーム名のUI
	float clearBerYPos = 0.0f;
	float clearBerYPosPram = 0.5f;
	std::unique_ptr<Sprite> clearBer_ = nullptr;
	// Texture番号
	uint32_t clearColorTex_ = 1;
	uint32_t pushATex_ = 1;
	uint32_t clearTex_ = 1;

private: //Audio
	uint32_t clearBGM_ = 0;
	uint32_t pushSE_ = 0;

private: // スコア
	std::unique_ptr<Score> score_ = nullptr;
	// 今回のスコア結果
	int32_t resultScore_ = 0;
};
;

