#include "Skydome.h"
#include "TextureManager.h"
#include "ModelManager.h"

void Skydome::Init()
{
	ModelManager::GetInstance()->LoadModel("Resources/skydome", "skydome.obj");
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1000.0f,1000.0f,1000.0f };
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetMaterial({ {1.0f,1.0f,1.0f,1.0f} , false });
	object_->SetModel("skydome.obj");
	object_->SetWorldTransform(worldTransform_);
	
	object_->Update();
	worldTransform_.UpdateMatrix();
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/skydome.png");
	object_->SetSkinTex(skinTex_);
}

void Skydome::Update()
{
	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
}

void Skydome::Draw(Camera* camera)
{
	object_->SetSkinTex(skinTex_);
	object_->Draw(camera);
}
