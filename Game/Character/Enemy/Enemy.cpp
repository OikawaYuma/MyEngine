#include "Enemy.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include <numbers>
#include "Character/Player/Player.h"
void Enemy::Init(const Vector3& translate, const std::string filename)
{
	ModelManager::GetInstance()->LoadModel("Resources/enemy","enemy.obj");
	;
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("player.obj");
	
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/enemy/pig.png");
	worldTransform_.translation_ = translate;

	SetCollisonAttribute(0b010);
	SetCollisionMask(0b001);
}

void Enemy::Update()
{
	Move();
	object_->SetWorldTransform(worldTransform_);
	object_->Update();
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(Camera* camera)
{
	object_->Draw(skinTex_,camera);
}

void Enemy::Move()
{

	
	//worldTransform_.rotation_.y += 0.02f * (float)std::numbers::pi;
	//worldTransform_.UpdateMatrix();

	//if (!(move.x == 0 && move.y == 0 && move.z == 0)) {
	//	move = Normalize(move);
	//	move.x *= 0.4f;
	//	move.y *= 0.4f;
	//	move.z *= 0.4f;
	//	move = TransformNormal(move, worldTransform_.matWorld_);
	//	// Y軸周り角度（Θy）
	//	//preAngle = std::atan2(move.x, move.z);

	//}
	//worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 重量加速度
	const float kGravityAcceleration = 0.1f;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0, -kGravityAcceleration, 0 };
	// 加速する
	move = Add(move, accelerationVector);

	// 着地
	if (worldTransform_.translation_.y <= 0.5f) {
		worldTransform_.translation_.y = 0.5f;
		// ジャンプ初速
		const float kJumpFirstSpeed = 1.0f;
		// ジャンプ初速を与える
		move.y = kJumpFirstSpeed;
	}
}

void Enemy::OnCollision(uint32_t attri)
{
	isDead_ = true;
	if (attri == 0b0001) {
		float playerHP = player_->GetHP();
		playerHP -= 0.1f;
		player_->SetHP(playerHP);

	}
}

Vector3 Enemy::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;// ワールド行列座標を入れる変数
}


