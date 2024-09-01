#pragma once
#include <memory>
#include "Camera.h"
#include "Character/base/BaseCharacter.h"
class Enemy : public BaseCharacter
{
public:
	void Init() override;
	void Update() override;
	void Draw(Camera* camera) override;

public:
	void Move();

private:
	std::unique_ptr<Object3d> object_;

};

