#include "WorldDesign.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Model.h"
#include "Object3dManager.h"
void WorldDesign::Init(const Vector3& scale, const Vector3& translate, const std::string filename)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/wood.png");
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.translation_ = translate;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	int i = Object3dManager::GetInstance()->StoreObject(filename,&worldTransform_,floorTex_,&color_);
	
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;

	worldTransform_.UpdateMatrix();

	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, filename);
	shadowObject_->Update();


	Object3dManager::GetInstance()->StoreObject(filename, shadowObject_->GetWorldTransform(), floorTex_, shadowObject_->GetColor());
}

void WorldDesign::Update()
{

	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void WorldDesign::Draw(Camera* camera)
{
	camera_ = camera;
	shadowObject_->Draw(camera_);
	object_->Draw(camera_);
}

