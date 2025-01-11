#include "Enemy.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include <numbers>
#include "Character/Player/Player.h"
#include <Object3dManager.h>

int Enemy::enemyDestory_ = 0;

void Enemy::Init(const Vector3& translate, const std::string filename)
{
	ModelManager::GetInstance()->LoadModel("Resources/slimeDead", "slimeDead.obj");
	ModelManager::GetInstance()->LoadModel("Resources/enemy","enemy.obj");;
	hp_ = 0.7f;
	glavity_ = 0;
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/" +filename + ".png");

	worldTransform_.translation_ = translate;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_,hp_,hp_ };
	worldTransform_.translation_.y = worldTransform_.scale_.y;
	Object3dManager::GetInstance()->StoreObject(filename, &worldTransform_, skinTex_, &color_);
	worldTransform_.UpdateMatrix();
	SetCollisonAttribute(0b010);
	SetCollisionMask(0b001);
	
	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, filename);
	shadowObject_->Update();
	Object3dManager::GetInstance()->StoreObject(filename, shadowObject_->GetWorldTransform(), skinTex_, shadowObject_->GetColor());
}

void Enemy::Update()
{
	Respown();
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_,hp_,hp_ };
	// 着地
	if (worldTransform_.translation_.y <= worldTransform_.scale_.y) {
		worldTransform_.translation_.y = worldTransform_.scale_.y;
	}
	Move();
	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void Enemy::Draw(Camera* camera)
{
	shadowObject_->Draw(camera);
	//object_->Draw(camera);
}

void Enemy::ClearInit()
{
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/slimeDead/enemySlimeDead.png");

	ModelManager::GetInstance()->LoadModel("Resources/slimeDead", "slimeDead.obj");
	deadSlimeObj_ = std::make_unique<Object3d>();
	deadSlimeObj_->Init();
	deadSlimeObj_->SetModel("slimeDead.obj");
	worldTransform_.translation_.y = 0.01f;
	worldTransform_.scale_ = { 3.0f,1.0f,3.0f };
	worldTransform_.UpdateMatrix();
	deadSlimeObj_->SetWorldTransform(worldTransform_);
	//object_->Update();
}

void Enemy::ClearUpdate()
{
	worldTransform_.UpdateMatrix();
	deadSlimeObj_->SetWorldTransform(worldTransform_);
	deadSlimeObj_->Update();
}

void Enemy::ClearDraw(Camera* camera)
{
	deadSlimeObj_->Draw(camera);
}

void Enemy::GameOverUpdate()
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
	worldTransform_.translation_ = Add(worldTransform_.translation_, {0.0f,move.y,0.0f});
	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void Enemy::Move()
{

	
	//worldTransform_.rotation_.y += 0.02f * (float)std::numbers::pi;
	//worldTransform_.UpdateMatrix();

	Vector2 directionToPlayer;
	directionToPlayer = Subtract(Vector2{ player_->GetWorldPosition().x ,player_->GetWorldPosition().z }, Vector2{ GetWorldPosition().x,GetWorldPosition().z });

	if (!(directionToPlayer.x == 0 && directionToPlayer.y == 0)) {
		directionToPlayer = Normalize(directionToPlayer);
		directionToPlayer.x *= 0.4f;
		directionToPlayer.y *= 0.4f;
		//move = TransformNormal(move, worldTransform_.matWorld_);
		// Y軸周り角度（Θy）
		//preAngle = std::atan2(move.x, move.z);
	}
	//worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	move.x = directionToPlayer.x;
	move.z = directionToPlayer.y;
	// 重量加速度
	const float kGravityAcceleration = 0.005f;
	glavity_ += kGravityAcceleration;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0, -glavity_, 0 };
	// 加速する
	move = Add(move, accelerationVector);

	// 着地
	if (worldTransform_.translation_.y <= worldTransform_.scale_.y) {
		worldTransform_.translation_.y = worldTransform_.scale_.y;
		// ジャンプ初速
		const float kJumpFirstSpeed = 2.0f;
		glavity_ = 0.0f;
		// ジャンプ初速を与える
		move.y = kJumpFirstSpeed;
	}
	move = TransformNormal(move, worldTransform_.matWorld_);
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::Respown()
{
	if (isDead_) {
		respownTimer_++;
		if (respownTimer_ >= 500) {
			hp_ = 0.7f;
			color_.w = 1.0f;
			shadowObject_->SetColor({ .w = 1.0f });
			respownTimer_ = 0;
			isDead_ = false;
			std::random_device seedGenerator;
			std::mt19937 randomEngine(seedGenerator());
			std::uniform_real_distribution<float> distriposX(-50, 50);// distriposX(-0.7f, -0.3
			std::uniform_real_distribution<float> distriposZ(-50, 50);// distriposY(0.2f, 0.5f)
			worldTransform_.translation_ = { distriposX(randomEngine),1.0f ,distriposZ(randomEngine) };
		}
	}
}

void Enemy::OnCollision(uint32_t attri)
{
	if (isDead_ == false) {
		if (attri == 0b0001) {
			player_->HitEnemySlime();
		}
		else if (attri == 0b1000) {
			hp_ -= 0.2f;
			worldTransform_.translation_.y -= 0.2f;
			if (hp_ <= 0.4f) {
				color_.w = 0.0f;
				worldTransform_.translation_.x = 100000.0f;
				shadowObject_->SetColor({ .w = 0.0f });
				isDead_ = true;
				enemyDestory_++;
			}

		}
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


