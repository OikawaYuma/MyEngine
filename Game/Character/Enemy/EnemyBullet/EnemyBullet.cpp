#include "EnemyBullet.h"
#include "Player.h"

void EnemyBullet::Init(const Vector3& pos, const Vector3& velocity)
{
	// NULLポインタチェック
	//assert(model);
	color = { 1.0f,1.0f,1.0f,1.0f };

	object3d_ = std::make_unique<Object3d>();
	object3d_->Init();
	object3d_->SetModel("box.obj");

	// テクスチャ読み込み
	textureHandle_ = TextureManager::StoreTexture("Resources/EnemyBullet.png");

	worldtransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldtransform_.translation_ = pos;
	worldtransform_.scale_ = { 0.5f,0.5f,0.5f };
	worldtransform_.UpdateMatrix();
	SetRadius(worldtransform_.scale_.x);
	velocity_ = velocity;
	SetCollisonAttribute(1);
	SetCollisionMask(0);
	material.color = color;
	material.enableLighting = true;
	material.shininess = 5.0f;
	dir_.direction = { 0.0f,-1.0f,0.0f };

	SetCollisonAttribute(0b010);
	SetCollisionMask(0b001);
}

void EnemyBullet::Update()
{
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

//	// 敵弾から自キャラへのベクトル計算
//	Vector3 toPlayer;
//	toPlayer.x = player_->GetWorldPosition().x - worldtransform_.matWorld_.m[3][0];
//	toPlayer.y = player_->GetWorldPosition().y - worldtransform_.matWorld_.m[3][1];
//	toPlayer.z = player_->GetWorldPosition().z - worldtransform_.matWorld_.m[3][2];
//
//	float t = 0.01f;
//
//	// 引数で受け取った速度をメンバ変数に代入
//	velocity_ = SLerp(toPlayer,
//		{ worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1],
//		worldtransform_.matWorld_.m[3][2] }, t);
//
//	velocity_.x *= 0.7f;
//	velocity_.y *= 0.7f;
//	velocity_.z *= 0.7f;
//
//	// Y軸周り角度（Θy）
//	worldtransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
//	float velocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
//
//	worldtransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);

	object3d_->Update();
	object3d_->SetWorldTransform(worldtransform_);

	worldtransform_.translation_ = Add(worldtransform_.translation_, velocity_);
	worldtransform_.UpdateMatrix();

	if (worldtransform_.translation_.y -worldtransform_.scale_.y <=0) {
		isDead_ = true;

	}

}

void EnemyBullet::Draw(Camera* camera)
{
	
	object3d_->Draw(textureHandle_,camera);
}

void EnemyBullet::OnCollision(uint32_t arri)
{
	isDead_ = true;
	if (arri == 0b0001) {
		float playerHP = player_->GetHP();
		playerHP -= 0.1f;
		player_->SetHP(playerHP);

	}
}

Vector3 EnemyBullet::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;
}