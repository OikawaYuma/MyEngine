#include "ClearCamera.h"
#include "ImGuiCommon.h"
#include "Audio.h"
void ClearCamera::Init()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ -3.0f,5.5f,192.6f });
	camera_->SetRotate({ 0.007f,0.0f,0.0f });

}

void ClearCamera::Update()
{


	Vector3 t = camera_->GetTranslate();
	Vector3 r = camera_->GetRotate();
	//t.z += 0.25f;
	/*ImGui::Begin("camera1");
	ImGui::DragFloat3("t",&t.x,0.10f);
	ImGui::DragFloat3("r", &r.x, 0.01f);
	ImGui::End();*/
	camera_->SetTranslate(t);
	camera_->SetRotate(r);
	camera_->Update();

}
