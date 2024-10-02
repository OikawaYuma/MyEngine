#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "Camera.h"
#include "Ground/Ground.h"
#include "Skydome/Skydome.h"
#include "Character/Player/TitlePlayer.h"
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
	Sprite* pushASp_ = nullptr;
	uint32_t titleTex_ = 1;
	uint32_t pushATex_ = 1;
	PostProcess* postProcess_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<TitlePlayer> player_;
};

