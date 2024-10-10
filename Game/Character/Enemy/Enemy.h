#pragma once
#include <memory>
#include "Camera.h"
#include "Character/base/BaseCharacter.h"
#include "EnemyBase.h"
class Player;
class Enemy: public EnemyBase
{
public:
	void Init(const Vector3& translate, const std::string filename) ;
	void Update() ;
	void Draw(Camera* camera) ;

public:
	void Move();

public: // Collider
	void OnCollision(uint32_t attri) override;
	 Vector3 GetWorldPosition() const override ;
	 bool IsDead() { return isDead_; }
	 void SetPlayer(Player* player) { player_ = player; }
private:
	std::unique_ptr<Object3d> object_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// model skin num
	uint32_t skinTex_ = 0;
	bool isDead_ = false;
	int radius_ = 1;
	Player* player_ = nullptr;
	Vector3 move{ 0,0,-0.1f };
};

