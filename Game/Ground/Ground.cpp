#include "Ground.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiCommon.h"

void Ground::Init(const Vector3& translate, const Vector3& scale, const std::string filename)
{
	worldTransform_.Initialize();
	worldTransform_.translation_ = translate;
	worldTransform_.scale_ = scale;
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel(filename + ".obj");
	object_->SetWorldTransform(worldTransform_);
	object_->Update();

	direLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	direLight_.direction = { 0.0f,-1.0f,0.0f };
	direLight_.intensity = 1.0f;
	object_->SetDirectionLight(direLight_);

	worldTransform_.UpdateMatrix();
	//object_->SetMaterial({ {1.0f,1.0f,1.0f,1.0f} , false });
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/ground/" + filename +".png");
	object_->SetSkinTex(skinTex_);
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
}

void Ground::Draw()
{
	object_->Draw(camera_);
}
