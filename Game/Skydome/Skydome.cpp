#include "Skydome.h"
#include "TextureManager.h"
#include "ModelManager.h"

void Skydome::Init()
{
	ModelManager::GetInstance()->LoadModel("Resources/skydome", "skydome.obj");
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100.0f,100.0f,100.0f };
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("skydome.obj");
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/uvChecker.png");
}

void Skydome::Update()
{
	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
}

void Skydome::Draw(Camera* camera)
{
	object_->Draw(skinTex_, camera);
}