#include "FollowCamera.h"
#include "Input.h"
#include "LockOn/LockOn.h"
void FollowCamera::Init()
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ 0,5,-30 });
	cameraTime_ = 1.0f;
}

void FollowCamera::Upadate()
{

	Vector3 cameraRotate = camera_->GetRotate();
	destinationAngleY_ = cameraRotate.y;
	
	if (lockOn_->GetTarget()) {
		Vector3 lockOnPos = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = Subtract(lockOnPos , target_->translation_);

		// Y軸回り角度
		cameraRotate.y = std::atan2(sub.x, sub.z);
	}
	else if (Input::GetInstance()->GetJoystickState()) {
		cameraRotate.y += Input::GetInstance()->JoyStickParmRX(0.03f);
		
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_RIGHT_THUMB)) {
			resetAngleFlag = true;
			resetAngleTime_ = 0;
			
		}
	}
	
	if (resetAngleFlag) {
		resetAngleTime_ += 0.1f;
		if (target_) {
			Vector3 offset = { 0,2,-30 };
			offset = TransformNormal(offset, target_->matWorld_);
			float resetAngle = std::atan2(offset.x, offset.z);
			cameraRotate.y =
				LerpShortAngle(cameraRotate.y, resetAngle + (float)std::numbers::pi, resetAngleTime_);
			
		}
		if (resetAngleTime_ > 1.0f) {
			resetAngleFlag = false;
		}
	}
	/*
	if (cameraRotate.y != destinationAngleY_) {
		angletime += 0.1f;
		cameraRotate.y =
			LerpShortAngle(cameraRotate.y, destinationAngleY_, angletime);
	}
	
	
	if (1.0f <= angletime) {
		angletime = 0.0f;
	}*/
	
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

	//// 追従対象が居れば
	//if (target_) {
	//	// 追従対象からカメラまでのオフセット
	//	Vector3 offset = { 0.0f,2.0f,-30.0f };


	//	offset = Transform1(offset, MakeRotateMatrix(camera_->GetRotate()));
	//	// 座標をコピーしてオフセット分ずらす

	//	camera_->SetTranslate(Add(target_->translation_, offset));
	//	camera_->SetTranslate({ camera_->GetTranslate().x,5,camera_->GetTranslate().z });

	//}
	// 追従対象からのオフセット
	if (target_) {
		Vector3 offset = { 0,2,-30 };
		offset = TransformNormal(offset, camera_->GetCameraMatrix());
		camera_->SetTranslate(Add(interarget_, offset));
		camera_->SetTranslate({ camera_->GetTranslate().x,5,camera_->GetTranslate().z });
	}
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

	Vector3 offset = { 0,2,-30 };
	offset = TransformNormal(offset, camera_->GetCameraMatrix());
	camera_->SetTranslate(Add(interarget_, offset));
	camera_->SetTranslate({ camera_->GetTranslate().x,5,camera_->GetTranslate().z });
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target; 
	Reset();
}


