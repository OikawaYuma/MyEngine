#include "GameOverCamera.h"
#include "ImGuiCommon.h"
#include "Audio.h"
void GameOverCamera::Init()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({4.0f,0.2f,214.2f});
	camera_->SetRotate({ -0.18f,-0.3f,0.16f });
	
}

void GameOverCamera::Update()
{

	
	Vector3 t = camera_->GetTranslate();
	Vector3 r = camera_->GetRotate();
	//t.z += 0.25f;
	/*ImGui::Begin("camera");
	ImGui::DragFloat3("t",&t.x,0.10f);
	ImGui::DragFloat3("r", &r.x, 0.01f);
	ImGui::End();*/
	camera_->SetTranslate(t);
	camera_->SetRotate(r);
	camera_->Update();

}