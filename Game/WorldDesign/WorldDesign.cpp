#include "WorldDesign.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Model.h"
void WorldDesign::Init(const Vector3& scale, const Vector3& translate, const std::string filename)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/floor/floor.png");
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = translate;

	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel(filename + ".obj");




}

void WorldDesign::Update()
{
	object_->Update();

	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
}

void WorldDesign::Draw(Camera* camera)
{
	camera_ = camera;
	object_->Draw(floorTex_, camera_);
}

