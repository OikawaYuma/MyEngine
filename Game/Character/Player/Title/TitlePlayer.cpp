#include "TitlePlayer.h"
#include <ModelManager.h>
#include <Object3dManager.h>
#include <TextureManager.h>

void TitlePlayer::Init()
{
	// モデルのロード
	ModelManager::GetInstance()->LoadModel("Resources/player", "player.obj");
	// ワールド座標の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,1.0f,100.0f };
	worldTransform_.translation_.y = 0.5f;
	// RGBA情報の初期化、Aはスライムらしさを出すため小さくする
	color_ = { 1.0f,1.0f,1.0f,0.8f, };
	// テクスチャをロードし格納番号を保持する。
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/player/player.png");
	// オブジェクトをマネジャーに積む
	Object3dManager::GetInstance()->StoreObject("player", &worldTransform_, skinTex_, &color_, Transparency::Transparent);
	// 影の初期化
	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, "player.obj");
	shadowObject_->Update();
	// 影オブジェクトをマネジャーに積む
	Object3dManager::GetInstance()->StoreObject("player", shadowObject_->GetWorldTransform(), skinTex_, shadowObject_->GetColor(), Transparency::Transparent);
}

void TitlePlayer::Update()
{
	// ジャンプ
	Jump();
	// 移動
	Move();
	// ワールド座標の更新
	worldTransform_.UpdateMatrix();
	// 影の更新 
	shadowObject_->Update();
}

void TitlePlayer::Move()
{
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velo_);
}

void TitlePlayer::Jump()
{
	
	// 重量加速度
	const float kGravityAcceleration = 0.1f;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0, -kGravityAcceleration, 0 };
	// 加速する
	velo_ = Add(velo_, accelerationVector);

	// 着地
	if (worldTransform_.translation_.y <= 0.5f ) {
		worldTransform_.translation_.y = 0.5f;
		// タイトル用なので無限にジャンプする
		worldTransform_.translation_.y = 2.0;
		// ジャンプ初速
		const float kJumpFirstSpeed = 1.0f;
		// ジャンプ初速を与える
		velo_.y = kJumpFirstSpeed;
	}
}
