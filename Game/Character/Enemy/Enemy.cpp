#include "Enemy.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include <numbers>
void Enemy::Init()
{
	ModelManager::GetInstance()->LoadModel("Resources/enemy","enemy.obj");
	;
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("enemy.obj");
	
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/enemy/pig.png");
	worldTransform_.translation_.z = 40.0f;
	worldTransform_.translation_.y = 2.0f;


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

	Vector3 move{ 0,0,2 };
	worldTransform_.rotation_.y += 0.02f * (float)std::numbers::pi;
	worldTransform_.UpdateMatrix();

	if (!(move.x == 0 && move.y == 0 && move.z == 0)) {
		move = Normalize(move);
		move.x *= 0.4f;
		move.y *= 0.4f;
		move.z *= 0.4f;
		move = TransformNormal(move, worldTransform_.matWorld_);
		// Y軸周り角度（Θy）
		//preAngle = std::atan2(move.x, move.z);

	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::OnCollision(uint32_t attri)
{
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


