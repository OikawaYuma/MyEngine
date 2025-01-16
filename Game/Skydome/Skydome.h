/**
* @ file Skydome.h
* @ brief 天球に関する設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"

class Skydome
{
public:
	void Init();
	void Update();
	void Draw(Camera* camera);



public: // Setter
	void SetSkydomeTexture(const uint32_t& tex) { skinTex_ = tex; }
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	std::unique_ptr<Object3d> object_;
	uint32_t skinTex_ = 0;
};

