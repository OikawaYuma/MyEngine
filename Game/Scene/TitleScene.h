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
#include "Slime2d/Slime2d.h"
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

	void DeleteObject();

private:
	Sprite* sprite = nullptr;

	Vector2 titleSpritePos_;
	float titleSpriteVelo_;

	std::unique_ptr<Sprite> LoadStringSp_;
	uint32_t LoadStringSpTex_ = 0;

	std::unique_ptr<Slime2d> slime2DSp1_;
	std::unique_ptr<Slime2d> slime2DSp2_;
	std::unique_ptr<Slime2d> slime2DSp3_;
	uint32_t jumpNum_ = JUMPONE;
	float loadpos = 0;

	bool loadingSpriteMoveFlag_ = false;

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
	std::list<std::unique_ptr<PlayerItem>> items_;
	std::list<std::unique_ptr<WorldDesign>> worldDesigns_;
	std::unique_ptr<Skydome> skydome_;

	float thre_;
	float threPorM_;
	bool threFlag_ = false;

	uint32_t titleBGM_;

	bool GamePlayFlag_ = false;
	uint32_t startTimer = 0;
	uint32_t startTimer2 = 0;
};

