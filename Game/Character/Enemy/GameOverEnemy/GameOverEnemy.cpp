#include "GameOverEnemy.h"
#include <ModelManager.h>
#include <Object3dManager.h>
#include <TextureManager.h>
#include "Character/Player/Player.h"
void GameOverEnemy::Init(const Vector3& translate, const std::string filename)
{
	filename;
	ModelManager::GetInstance()->LoadModel("Resources/slimeDead", "slimeDead.obj");
	ModelManager::GetInstance()->LoadModel("Resources/enemy", "enemy.obj");;
	hp_ = 0.7f;
	glavity_ = 0;
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/enemy.png");

	worldTransform_.translation_ = translate;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_,hp_,hp_ };
	worldTransform_.translation_.y = worldTransform_.scale_.y;
	Object3dManager::GetInstance()->StoreObject("enemy", &worldTransform_, skinTex_, &color_, Transparency::Opaque);
	worldTransform_.UpdateMatrix();
	SetCollisonAttribute(0b010);
	SetCollisionMask(0b001);

	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, "enemy");
	shadowObject_->Update();
	Object3dManager::GetInstance()->StoreObject("enemy", shadowObject_->GetWorldTransform(), skinTex_, shadowObject_->GetColor(), Transparency::Opaque);
}

void GameOverEnemy::Update()
{
	// 重量加速度
	const float kGravityAcceleration = 0.075f;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0, -kGravityAcceleration, 0 };
	// 加速する
	move = Add(move, accelerationVector);
	// 着地
	if (worldTransform_.translation_.y <= worldTransform_.scale_.y) {
		worldTransform_.translation_.y = worldTransform_.scale_.y;
		// ジャンプ初速
		const float kJumpFirstSpeed = 1.0f;
		// ジャンプ初速を与える
		move.y = kJumpFirstSpeed;
	}
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, { 0.0f,move.y,0.0f });
	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void GameOverEnemy::OnCollision(uint32_t attri)
{
	attri;
}
Vector3 GameOverEnemy::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;// ワールド行列座標を入れる変数
}
