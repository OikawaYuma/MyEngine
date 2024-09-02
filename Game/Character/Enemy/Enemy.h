#pragma once
#include <memory>
#include "Camera.h"
#include "Character/base/BaseCharacter.h"
class Enemy: public Collider
{
public:
	void Init() ;
	void Update() ;
	void Draw(Camera* camera) ;

public:
	void Move();

public: // Collider
	void OnCollision(uint32_t attri) override;
	 Vector3 GetWorldPosition() const override ;


private:
	std::unique_ptr<Object3d> object_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// model skin num
	uint32_t skinTex_;
};

