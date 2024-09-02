#include "PlayerBullet.h"
#include "TextureManager.h"

//#include "GameScene.h"
void PlayerBullet::Init(const Vector3& pos, const Vector3& velocity)
{
	// NULLポインタチェック
	//assert(model);
	material.color = color;
	material.enableLighting = true;
	material.shininess = 5.0f;
	dir_.direction = { 0.0f,-1.0f,0.0f };

	texHandle_ = TextureManager::StoreTexture("Resources/playerBullet.png");
	
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("player.obj");

	// テクスチャ読み込み
	//textureHandle_ = TextureManager::StoreTexture("Resources/uvChecker.png");

	worldtransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldtransform_.translation_ = pos;
	worldtransform_.scale_ = { 0.5f,0.5f,0.5f };
	SetRadius(worldtransform_.scale_.x);
	worldtransform_.UpdateMatrix();
	velocity_ = velocity;
	// 衝突属性を設定
	SetCollisonAttribute(0b1000);

	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0b0010);
}

void PlayerBullet::Update()
{


	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldtransform_.translation_ = Add(worldtransform_.translation_, velocity_);
	worldtransform_.UpdateMatrix();
	object_->SetWorldTransform(worldtransform_);
	if (worldtransform_.translation_.y - worldtransform_.scale_.y <= 0) {
		isDead_ = true;

	}
}

void PlayerBullet::Draw(Camera* camera)
{
	object_->Draw(texHandle_,camera);
}

void PlayerBullet::SetParent(const WorldTransform* parent)
{
	// 親子関係を結ぶ
	worldtransform_.parent_ = parent;
}

void PlayerBullet::OnCollision(uint32_t attri)
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;
}