#include "TitleCamera.h"
#include "ImGuiCommon.h"

void TitleCamera::Init()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetFarClip(2000.0f);
	switch (cameraType_) {
	case CAMERA1:
		camera_->SetTranslate({ 23,0.4f,117.5f });
		camera_->SetRotate({ -0.21f,-2.14f,0.0f });
		break;
	case CAMERA2:
		camera_->SetTranslate({ 23,0.4f,117.5f });
		camera_->SetRotate({ -0.21f,-2.14f,0.0f });
		break;
	case CAMERA3:
		camera_->SetTranslate({ 23,0.4f,117.5f });
		camera_->SetRotate({ -0.21f,-2.14f,0.0f });
		break;
	}


}

void TitleCamera::Update()
{

	cameraTimer_++;
	if (cameraTimer_ >= 240) {
		switch (cameraType_) {
		case CAMERA1:
			camera_->SetTranslate({ -38.2f,5.9f,99.5f });
			camera_->SetRotate({ 0.0f,1.57f,0.0f });
			cameraType_ = CAMERA2;
			break;
		case CAMERA2:
			camera_->SetTranslate({ 0,16.3f,39.2f });
			camera_->SetRotate({ 0.11f,0.0f,0.0f });
			cameraType_ = CAMERA3;
			break;
		case CAMERA3:
			
			camera_->SetTranslate({ 23,0.4f,117.5f });
			camera_->SetRotate({ -0.21f,-2.14f,0.0f });
			cameraType_ = CAMERA1;
			break;
		}
		cameraTimer_ = 0;
	}
	Vector3 t = camera_->GetTranslate();
	Vector3 r = camera_->GetRotate();
	t.z += 0.25f;
	/*ImGui::Begin("camera");
	ImGui::DragFloat3("t",&t.x,0.10f);
	ImGui::DragFloat3("r", &r.x, 0.01f);
	ImGui::End();*/
	camera_->SetTranslate(t);
	camera_->SetRotate(r);
	camera_->Update();
	
}
