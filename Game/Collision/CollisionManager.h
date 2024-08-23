#pragma once
#include "Collider.h"
#include<list>
#include <memory>
class GameScene;
class Player;
class Enemy;
class EnemyBullet;
class CollisionManager {
public:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	void CheckAllCollision();
	// ClliderにPushする
	void PushClider(Collider* collider);
	void SetGameScene(GameScene* gamescene) { gameScene_ = gamescene; }
	void SetPlayer(Player* player) { player_ = player; }
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	void SetEnemyBullet(EnemyBullet* enemy) { enemyBullet_ = enemy; }
private:
	GameScene* gameScene_ = nullptr;
	//std::list<Collider*> colliders_;
	// 自キャラ
	Player* player_ = nullptr;

	// 敵キャラ
	Enemy* enemy_ = nullptr;

	EnemyBullet* enemyBullet_ = nullptr;
	uint32_t collisionNum = 2;
};

