#include "PlayerItem.h"
#include "Character/Player/Player.h"
void PlayerItem::Init(const Vector3& translate, const std::string filename)
{

	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/item.png");
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 1.0f;
	worldTransform_.scale_.z = 1.0f;
	worldTransform_.translation_.x = translate.x;
	worldTransform_.translation_.y = translate.y;
	worldTransform_.translation_.z = translate.z;


	//ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel(filename + ".obj");
	object_->SetWorldTransform(worldTransform_);
	object_->Update();
	object_->SetSkinTex(floorTex_);
	worldTransform_.UpdateMatrix();
	SetRadius(1.0f);
	SetCollisonAttribute(0b0100);
	SetCollisionMask(0b0001);
	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, filename + ".obj");
	shadowObject_->Update();
}

void PlayerItem::Update()
{
	object_->Update();
	object_->SetWorldTransform(worldTransform_);

	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void PlayerItem::Draw(Camera* camera)
{
	shadowObject_->Draw(camera);
	object_->Draw(camera);
}

void PlayerItem::OnCollision(uint32_t attri)
{
	if (attri) {

	}
	isDead_ = true;
	float preScale = player_->GetHP();
	preScale += 0.2f;
	player_->SetHP(preScale);

}


Vector3 PlayerItem::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
