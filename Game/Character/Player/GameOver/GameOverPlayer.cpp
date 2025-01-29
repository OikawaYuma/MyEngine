#include "GameOverPlayer.h"
#include <ModelManager.h>
#include <Object3dManager.h>
#include <TextureManager.h>

void GameOverPlayer::Init()
{
	// モデルのロード
	ModelManager::GetInstance()->LoadModel("Resources/slimeDead", "slimeDead.obj");
	// ワールド座標の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { -1.0f,0.01f,221.0f };
	worldTransform_.scale_ = { 5.0f,1.0f,5.0f };
	worldTransform_.UpdateMatrix();
	// RGBA情報の初期化、Aはスライムらしさを出すため小さくする
	color_ = { 1.0f,1.0f,1.0f,0.8f };
	// テクスチャをロードし格納番号を保持する。
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/slimeDead/slimeDead.png");
	// 影オブジェクトをマネジャーに積む
	Object3dManager::GetInstance()->StoreObject("slimeDead", &worldTransform_, skinTex_, &color_, Transparency::Transparent);
}
