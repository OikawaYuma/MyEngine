/**
* @ file
* @ brief
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
#include "Character/Enemy/Enemy.h"
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
private:
	uint32_t titleTex_ = 1;
	PostProcess* postProcess_ = nullptr;
	std::unique_ptr<GameOverCamera> camera_ = nullptr;

	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<Ground> ground_;
	std::list<std::unique_ptr<PlayerItem>> items_;
	std::list<std::unique_ptr<WorldDesign>> worldDesigns_;
	std::unique_ptr<Skydome> skydome_;

	float thre_;
	float threPorM_;
	bool threFlag_ = false;
	bool titleFlag_ = false;

	std::unique_ptr<Sprite> pushASp_ = nullptr;
	float pushSpriteAlpha_;
	float pushSpriteAlphaPorM_;

	float titleBerYPos = 0.0f;
	float titleBerYPosPram = 0.5f;
	std::unique_ptr<Sprite> titleBer_ = nullptr;
	uint32_t pushATex_ = 1;
	uint32_t gameOverTex_ = 1;

	uint32_t gameOverBGM_ = 0;

	uint32_t pushSE_ = 0;
};

