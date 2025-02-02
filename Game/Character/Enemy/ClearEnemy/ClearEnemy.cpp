#include "ClearEnemy.h"
#include <ModelManager.h>
#include <Object3dManager.h>
#include <TextureManager.h>
#include "Character/Player/Player.h"
void ClearEnemy::Init(const Vector3& translate, const std::string filename)
{
	filename;
	// モデルのロード
	ModelManager::GetInstance()->LoadModel("Resources/slimeDead", "slimeDead.obj");
	// RGBA情報の初期化、Aはスライムらしさを出すため小さくする
	color_ = { 1.0f,1.0f,1.0f,0.8f, };
	// テクスチャをロードし格納番号を保持する。
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/slimeDead/enemySlimeDead.png");
	// ワールド座標の初期化
	worldTransform_.translation_ = translate;
	worldTransform_.translation_.y = 0.01f;
	worldTransform_.scale_ = { 3.0f,1.0f,3.0f };
	worldTransform_.UpdateMatrix();
	// オブジェクトをマネジャーに積む
	Object3dManager::GetInstance()->StoreObject("slimeDead", &worldTransform_, skinTex_, &color_, Transparency::Transparent);
}

void ClearEnemy::Update()
{
}

void ClearEnemy::OnCollision(uint32_t attri)
{
	attri;
}
Vector3 ClearEnemy::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;// ワールド行列座標を入れる変数
}