#include "Ground.h"
#include "TextureManager.h"
#include "ModelManager.h"


void Ground::Init()
{
	ModelManager::GetInstance()->LoadModel("Resources/ground", "ground.obj");
	worldTransform_.Initialize();
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("ground.obj");
	//object_->SetMaterial({ {1.0f,1.0f,1.0f,1.0f} , false });
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/ground/ground.png");
}

void Ground::Update()
{
	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
}

void Ground::Draw()
{
	object_->Draw(skinTex_, camera_);
}
