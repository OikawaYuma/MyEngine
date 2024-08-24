#include "FollowCamera.h"
#include "Input.h"
void FollowCamera::Init()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ 0,5,-30 });
}

void FollowCamera::Upadate()
{

	Vector3 cameraRotate = camera_->GetRotate();
	if (Input::GetInstance()->GetJoystickState()) {
		cameraRotate.y += (float)Input::GetInstance()->GetJoyState().Gamepad.sThumbRX / SHRT_MAX * 0.05f;
	}
	camera_->SetRotate(cameraRotate);

	// 追従対象が居れば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f,2.0f,-30.0f };

		
		offset = Transform1(offset, MakeRotateMatrix(camera_->GetRotate()));
		// 座標をコピーしてオフセット分ずらす
		camera_->SetTranslate(Add(target_->translation_, offset));

	}
	

	camera_->Update();
}

