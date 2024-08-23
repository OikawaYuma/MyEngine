#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "Camera.h"
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
private:
	Sprite* sprite = nullptr;
	uint32_t titleTex_ = 1;
	PostProcess* postProcess_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;
};
;

