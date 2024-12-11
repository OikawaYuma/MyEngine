#include "PlayerItem.h"
#include "Character/Player/Player.h"
#include <Object3dManager.h>
void PlayerItem::Init(const Vector3& translate, const std::string filename)
{

	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/item.png");
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 1.0f;
	worldTransform_.scale_.z = 1.0f;
	worldTransform_.translation_.x = translate.x;
	worldTransform_.translation_.y = translate.y;
	worldTransform_.translation_.z = translate.z;
	baseHighPos_ = translate.y;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	//ModelManager::GetInstance()->LoadModel("Resources/box/", "box.obj");
	Object3dManager::GetInstance()->StoreObject(filename, &worldTransform_, floorTex_, &color_);
	worldTransform_.UpdateMatrix();
	SetRadius(1.0f);
	SetCollisonAttribute(0b0100);
	SetCollisionMask(0b0001);
	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, filename);
	shadowObject_->Update();
	Object3dManager::GetInstance()->StoreObject(filename, shadowObject_->GetWorldTransform(), floorTex_, shadowObject_->GetColor());
}

void PlayerItem::Update()
{
	Respown();
	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void PlayerItem::Draw(Camera* camera)
{
	shadowObject_->Draw(camera);
	object_->Draw(camera);
}

void PlayerItem::Respown()
{
	if (isDead_) {
		respownTimer_++;
		if (respownTimer_ >= 500) {
			respownTimer_ = 0;
			color_.w = 1.0f;
			shadowObject_->SetColor({ .w = 1.0f });

			isDead_ = false;
			std::random_device seedGenerator;
			std::mt19937 randomEngine(seedGenerator());
			std::uniform_real_distribution<float> distriposX(-50, 50);// distriposX(-0.7f, -0.3
			std::uniform_real_distribution<float> distriposZ(-50, 50);// distriposY(0.2f, 0.5f)
			worldTransform_.translation_ = { distriposX(randomEngine),baseHighPos_ ,distriposZ(randomEngine) };
		}
	}
}

void PlayerItem::OnCollision(uint32_t attri)
{
	if (attri) {

	}
	if (isDead_ == false) {
		isDead_ = true;
		shadowObject_->SetColor({.w = 0.0f});
		color_.w = 0.0f;
		float preScale = player_->GetHP();
		//worldTransform_.translation_.y = 100.0f;
		preScale += 0.2f;
		player_->SetHP(preScale);
	}

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
