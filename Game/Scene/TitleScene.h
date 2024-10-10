#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "GameCamera/TitleCamera/TitleCamera.h"
#include "Ground/Ground.h"
#include "Skydome/Skydome.h"
#include "Character/Player/Player.h"
#include "Character/Enemy/Enemy.h"
#include "Item/PlayerItem.h"
#include "WorldDesign/WorldDesign.h"

class TitleScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Draw2d()override;
	void Release()override;
	int GameClose()override;

private:
	Sprite* sprite = nullptr;
	Vector2 titleSpritePos_;
	float titleSpriteVelo_;

	Sprite* pushASp_ = nullptr;
	float pushSpriteAlpha_;
	float pushSpriteAlphaPorM_;

	uint32_t titleTex_ = 1;
	uint32_t pushATex_ = 1;
	PostProcess* postProcess_ = nullptr;
	std::unique_ptr<TitleCamera> camera_ = nullptr;

	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<Ground> ground_;
	std::list<PlayerItem*> items_;
	std::list<WorldDesign*> worldDesigns_;
	std::unique_ptr<Skydome> skydome_;

	float thre_;
	float threPorM_;
	bool threFlag_ = false;

	uint32_t titleBGM_;

	bool GamePlayFlag_ = false;
};

