#include "FollowCamera.h"
#include "Input.h"
#include "LockOn/LockOn.h"
#include <iostream>
#include <cmath>
void FollowCamera::Init()
{

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ 0,5,-30 });
	camera_->SetFarClip(2000.0f);
	cameraTime_ = 1.0f;
	offsetZ_ = -30.0f;

}

void FollowCamera::StartCameraEffect()
{
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

void FollowCamera::Upadate()
{

	Vector3 cameraRotate = camera_->GetRotate();
	destinationAngleY_ = cameraRotate.y;


	if (Input::GetInstance()->GetJoystickState()) {
		cameraRotate.y += Input::GetInstance()->JoyStickParmRX(0.03f);


	}



	camera_->SetRotate(cameraRotate);

	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y)) {
		cameraTime_ = 0;
		isDash_ = true;
	}
	if (target_) {
		// 追従座標の補間
		if (isDash_) {
			cameraTime_ += 0.01f;
			if (cameraTime_ > 1.0f) {
				cameraTime_ = 1.0f;
				isDash_ = false;
			}
		}
		interarget_ = Lerp(interarget_, target_->translation_, cameraTime_);
	}




	// 追従対象からのオフセット
	if (target_) {
		Vector3 offset = { 0,2,offsetZ_ };
		offset = TransformNormal(offset, camera_->GetCameraMatrix());
		camera_->SetTranslate(Add(interarget_, offset));
		camera_->SetTranslate({ camera_->GetTranslate().x,5,camera_->GetTranslate().z });
	}

	EndPosAdustment();


	camera_->Update();
}

void FollowCamera::Reset()
{
	if (target_) {
		// 追従座標・角度の初期化
		interarget_ = target_->translation_;
		Vector3 cameraRotate = camera_->GetRotate();

		camera_->SetRotate({
			cameraRotate.x,
			target_->rotation_.y,
			cameraRotate.z
			}
		);
	}
	destinationAngleY_ = camera_->GetRotate().y;

	Vector3 offset = { 0,2,offsetZ_ };
	offset = TransformNormal(offset, camera_->GetCameraMatrix());
	camera_->SetTranslate(Add(interarget_, offset));
	camera_->SetTranslate({ camera_->GetTranslate().x,5,camera_->GetTranslate().z });
}

void FollowCamera::PosAdustment()
{
	if (target_) {
		Vector3 offset = { 0,2,	offsetZ_ };
		offset = TransformNormal(offset, camera_->GetCameraMatrix());
		camera_->SetTranslate(Add(target_->translation_, offset));
		camera_->SetTranslate({ camera_->GetTranslate().x,5,camera_->GetTranslate().z });
	}
	camera_->Update();
}

void FollowCamera::EndPosAdustment()
{

	Vector3 cameraTranslate = camera_->GetTranslate();
	if (cameraTranslate.x >= 140.0f) {
		
		if (cameraTranslate.x > 160.0f) {
			cameraTranslate.x = 160.0f;
		}
		float y = Lerp(5, 1.3f, (cameraTranslate.x - 140) / (160 - 140));
		offsetZ_ = Lerp(-30, -12, (cameraTranslate.x - 140) / (160 - 140));
		cameraTranslate.y = y;
		
	}
	else if (cameraTranslate.x <= -140.0f) {
		if (cameraTranslate.x < -160.0f) {
			cameraTranslate.x = -160.0f;
			
		}
		float y = Lerp(5, 1.3f, (std::fabs(cameraTranslate.x) - 140) / (160 - 140));
		offsetZ_ = Lerp(-30, -12, (std::fabs(cameraTranslate.x) - 140) / (160 - 140));
		cameraTranslate.y = y;
	}
	

	if (cameraTranslate.z >= 140.0f) {

		if (cameraTranslate.z > 160.0f) {
			cameraTranslate.z = 160.0f;
	
		}
		float y = Lerp(5, 1.3f, (cameraTranslate.z - 140) / (160 - 140));
		offsetZ_ = Lerp(-30, -12, (cameraTranslate.z - 140) / (160 - 140));
		cameraTranslate.y = y;

	}
	else if (cameraTranslate.z <= -140.0f) {
		if (cameraTranslate.z < -160.0f) {
			cameraTranslate.z = -160.0f;
		
		}
		float y = Lerp(5, 1.3f, (std::fabs(cameraTranslate.z) - 140) / (160 - 140));
		offsetZ_ = Lerp(-30, -12, (std::fabs(cameraTranslate.z) - 140) / (160 - 140));
		cameraTranslate.y = y;
	}
	camera_->SetTranslate(cameraTranslate);
	if (
		cameraTranslate.z > -140.0f && cameraTranslate.z < 140.0f&&
		cameraTranslate.x > -140.0f && cameraTranslate.x < 140.0f) {
		offsetZ_ = -30.0f;
	}
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}


