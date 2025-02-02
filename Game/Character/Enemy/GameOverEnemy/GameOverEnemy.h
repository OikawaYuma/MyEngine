/**
* @ file GameOverEnemy.h
* @ brief GameOverSceneで出現する演出用敵Objectの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/
#include "Character/Enemy/BaseEnemy.h"
#include "Vector4.h"
#include "WorldTransform.h"
#include "PlaneProjectionShadow.h"
#pragma once
class Player;
class GameOverEnemy : public BaseEnemy
{
public:
	void Init(const Vector3& translate, const std::string filename) override;
	void Update() override;

public: //Setter
	void SetPlayer(Player* player) override { player_ = player; }

public:// Getter
	Vector3 GetWorldPosition() const override;

public: // Collider
	void OnCollision(uint32_t attri) override;

private:
	// ワールド座標
	WorldTransform worldTransform_{};
	// RGBA情報
	Vector4 color_{};
	// テクスチャ番号
	uint32_t skinTex_ = 0;
	// 速度
	Vector3 velo_{ 0.0f,0.0f,0.0f };
	// Playerのptr
	Player* player_ = nullptr;
	// 平行影
	std::unique_ptr<PlaneProjectionShadow> shadowObject_;
	// 重力
	float glavity_ = 0.0f;
	// 体力
	float hp_ = 1.0f;
	// 移動量
	Vector3 move{ 0,0,-0.1f };
};

