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
	object_->SetSpotlightPos(translate);
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;
	object_->SetMaterial(material_);

	/*spotLight_.color = { -3.7f,-3.7f,-3.7f,1.0f };
	spotLight_.position = worldTransform_.translation_;
	spotLight_.distance = 7.0f;
	spotLight_.direction =
		Normalize(Vector3{ -0.372f,-0.893f,0.253f });
	spotLight_.intensity = 39.1f;
	spotLight_.dacya = 4.1f;
	spotLight_.cosAngle =
		std::cos(std::numbers::pi_v<float> / 3.0f);*/
	/*object_->SetSpotLight(spotLight_);*/
	object_->SetWorldTransform(worldTransform_);
	object_->Update();
	worldTransform_.UpdateMatrix();

	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, filename + ".obj");
	shadowObject_->Update();
}

void WorldDesign::Update()
{
	object_->SetSpotLight(spotLight_);
	object_->Update();

	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
	shadowObject_->Update();
}

void WorldDesign::Draw(Camera* camera)
{
	camera_ = camera;
	shadowObject_->Draw(camera_);
	object_->Draw(floorTex_, camera_);
}

