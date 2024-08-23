#pragma once
#include"Vector3.h"
#include<cstdint>
enum CollisionMode {
	AABBc,
	OBBc,
	Ballc,
};

class Collider {
public:


	// Getter
	// 半径を取得
	float GetRadius() { return radius_; }

	// Setter
	void SetRadius(float radius) { radius_ = radius; }

	virtual void OnCollision(uint32_t attri) = 0;

	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() const = 0;

	// 衝突属性（自分）を取得
	uint32_t GetCollisonAttribute() { return collisionAttribute_; }
	// 衝突属性（自分）を設定
	void SetCollisonAttribute(uint32_t collisionAttribute);
	// 衝突マスク（相手）を取得
	uint32_t GetCollisionMask() { return CollisionMask_; }
	// 衝突マスク（相手）を設定
	void SetCollisionMask(uint32_t CollisionMask);

	// CollisionModeを取得
	uint32_t GetCollisionMode() { return collisionMode_; }

	void SetCollisionMode(uint32_t collisionMode) { collisionMode_ = collisionMode; }

private:
	// 衝突半径
	float radius_ = 2;

	uint32_t collisionMode_ = Ballc;

	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xFFFFFFFF;
	// 衝突マスク（相手）
	uint32_t CollisionMask_ = 0xffffffff;

	
};

