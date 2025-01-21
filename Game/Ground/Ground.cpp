#include "Ground.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiCommon.h"

void Ground::Init(const Vector3& translate, const Vector3& scale, const std::string filename)
{
	worldTransform_.Initialize();
	worldTransform_.translation_ = translate;
	worldTransform_.scale_ = scale;

	worldTransform_background.Initialize();
	worldTransform_background.translation_ = {0.0f,-4.5f,0.0f};
	worldTransform_background.scale_ = {500.0f,1.0f,500.0f};
	worldTransform_background.UpdateMatrix();
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel(filename + ".obj");
	object_->SetWorldTransform(worldTransform_);
	object_->Update();

	ModelManager::GetInstance()->LoadModel("Resources/ground","background.obj");
	backgroundObject_ = std::make_unique<Object3d>();
	backgroundObject_->Init();
	backgroundObject_->SetModel("background.obj");
	backgroundObject_->SetWorldTransform(worldTransform_background);
	backgroundObject_->Update();

	direLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	direLight_.direction = { 0.0f,-1.0f,0.0f };
	direLight_.intensity = 1.0f;
	object_->SetDirectionLight(direLight_);
	backgroundObject_->SetDirectionLight(direLight_);
	worldTransform_.UpdateMatrix();
	//object_->SetMaterial({ {1.0f,1.0f,1.0f,1.0f} , false });
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/ground/" + filename +".png");
	object_->SetSkinTex(skinTex_);

	skinTex_background = TextureManager::GetInstance()->StoreTexture("Resources/ground/" + filename + "2.png");
	backgroundObject_->SetSkinTex(skinTex_background);
}

void Ground::Update()
{
	/*ImGui::Begin("groundpos");
	ImGui::DragFloat("posx", &worldTransform_.translation_.z, 10.0f);
	ImGui::End();*/
	object_->SetSpotLight(spotLight_);
	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
	object_->Update();
	backgroundObject_->SetWorldTransform(worldTransform_background);
	backgroundObject_->Update();
}

void Ground::Draw()
{

	backgroundObject_->Draw(camera_);
	object_->Draw(camera_);
}
