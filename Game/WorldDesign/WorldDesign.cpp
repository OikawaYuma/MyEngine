#include "WorldDesign.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Model.h"
void WorldDesign::Init(const Vector3& scale, const Vector3& translate, const std::string filename)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/wood.png");
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = translate;

	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel(filename + ".obj");
	
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;
	object_->SetMaterial(material_);
	direLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	direLight_.direction = { 0.0f,-1.0f,0.0f };
	direLight_.intensity = 0.6f;
	object_->SetDirectionLight(direLight_);
	object_->SetWorldTransform(worldTransform_);
	object_->Update();
	worldTransform_.UpdateMatrix();

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

