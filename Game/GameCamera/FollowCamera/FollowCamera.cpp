#include "FollowCamera.h"
#include "Input.h"
#include "LockOn/LockOn.h"
#include <iostream>
#include <cmath>
#include <numbers>
void FollowCamera::Init()
{

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ 0,5,-30 });
	camera_->SetFarClip(2000.0f);
	cameraTime_ = 1.0f;
	offsetZ_ = -30.0f;
	cameraDirection_ = NONEDIRECTION;
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
	ImGui::Begin("CameraMove");
	ImGui::Text("Mode : %d", cameraDirection_);
	ImGui::Text("Timer : %f", cameraChanegeTimer_);

	ImGui::End();
	Vector3 cameraRotate = camera_->GetRotate();
	destinationAngleY_ = cameraRotate.y;

	// カメラが操作できる状態ならカメラを以下の値で回転する
	if (Input::GetInstance()->GetJoystickState()) {
		cameraRotate.y += Input::GetInstance()->JoyStickParmRX(0.03f) * std::numbers::pi_v<float>;
		if (cameraRotate.y >= 2.0f * std::numbers::pi_v<float>) {
			cameraRotate.y -= 2.0f * std::numbers::pi_v<float>;
		}
		else if (cameraRotate.y <= -2.0f * std::numbers::pi_v<float>) {
			cameraRotate.y += 2.0f * std::numbers::pi_v<float>;
		}

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
		camera_->SetTranslate(camera_->GetTranslate());
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
	camera_->SetTranslate(camera_->GetTranslate());
}

void FollowCamera::PosAdustment()
{
	if (target_) {
		Vector3 offset = { 0,2,	offsetZ_ };
		offset = TransformNormal(offset, camera_->GetCameraMatrix());
		camera_->SetTranslate(Add(target_->translation_, offset));
		camera_->SetTranslate(camera_->GetTranslate());
	}
	camera_->Update();
}

void FollowCamera::EndPosAdustment()
{

	Vector3 cameraTranslate = camera_->GetTranslate();
	
	preOffsetZ_ = offsetZ_;
	// X軸補間
	if (cameraTranslate.x >= 140.0f) {
		if (cameraDirection_ == NONEDIRECTION) {
			cameraDirection_ = XP;
		}
		if (cameraDirection_ == XP) {
			// X軸が160を超えないように制限
			if (cameraTranslate.x > 160.0f) {
				cameraTranslate.x = 160.0f;
			}

			// X軸の補間に対応するY軸の補間
			float y = Lerp(5, 1.3f, (cameraTranslate.x - 140) / (160 - 140));
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(-30, -12, (cameraTranslate.x - 140) / (160 - 140));
			preCameraTranslate_ = cameraTranslate;
		}
		else if (cameraDirection_ == ZP) {
			cameraChanegeTimer_ += 0.02f;
			// X軸が160を超えないように制限
			if (cameraTranslate.x > 160.0f) {
				cameraTranslate.x = 160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (preCameraTranslate_.z - 140) / (160 - 140)), Lerp(5, 1.3f, (cameraTranslate.x - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (preCameraTranslate_.z - 140) / (160 - 140)), Lerp(-30, -12, (cameraTranslate.x - 140) / (160 - 140)) , cameraChanegeTimer_);
			
			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = XP;
				cameraChanegeTimer_ = 0.0f;
			}
		}
		if (cameraDirection_ == ZM) {
			cameraChanegeTimer_ += 0.02f;
			// X軸が160を超えないように制限
			if (cameraTranslate.x > 160.0f) {
				cameraTranslate.x = 160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), Lerp(5,1.3f, (cameraTranslate.x - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), Lerp(-30, -12, (cameraTranslate.x - 140) / (160 - 140)), cameraChanegeTimer_);
			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = XP;
				cameraChanegeTimer_ = 0.0f;
			}
		}
	}
	else if (cameraTranslate.x <= -140.0f) {
		
		if (cameraDirection_ == NONEDIRECTION) {
			cameraDirection_ = XM;
		}
		if (cameraDirection_ == XM) {
			// X軸が-160を下回らないように制限
			if (cameraTranslate.x < -160.0f) {
				cameraTranslate.x = -160.0f;
			}

			// X軸の補間に対応するY軸の補間
			float y = Lerp(5, 1.3f, (std::fabs(cameraTranslate.x) - 140) / (160 - 140));
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(-30, -12, (std::fabs(cameraTranslate.x) - 140) / (160 - 140));
			preCameraTranslate_ = cameraTranslate;
		}
		else if (cameraDirection_ == ZP) {
			cameraChanegeTimer_ += 0.02f;
			// X軸が-160を下回らないように制限
			if (cameraTranslate.x < -160.0f) {
				cameraTranslate.x = -160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (preCameraTranslate_.z - 140) / (160 - 140)), Lerp(5, 1.3f, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (preCameraTranslate_.z - 140) / (160 - 140)), Lerp(-30, -12, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), cameraChanegeTimer_);

			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = XM;
				cameraChanegeTimer_ = 0.0f;
			}
		}
		if (cameraDirection_ == ZM) {
			cameraChanegeTimer_ += 0.02f;
			// X軸が-160を下回らないように制限
			if (cameraTranslate.x < -160.0f) {
				cameraTranslate.x = -160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), Lerp(5, 1.3f, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), Lerp(-30, -12, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), cameraChanegeTimer_);
			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = XM;
				cameraChanegeTimer_ = 0.0f;
			}
		}

	}

	// Z軸補間
	else if (cameraTranslate.z >= 140.0f) {
		if (cameraDirection_ == NONEDIRECTION) {
			cameraDirection_ = ZP;
		}

		
		if (cameraDirection_ == ZP) {
			// X軸が160を超えないように制限
			if (cameraTranslate.z > 160.0f) {
				cameraTranslate.z = 160.0f;
			}

			// X軸の補間に対応するY軸の補間
			float y = Lerp(5, 1.3f, (cameraTranslate.z - 140) / (160 - 140));
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(-30, -12, (cameraTranslate.z - 140) / (160 - 140));
			preCameraTranslate_ = cameraTranslate;
		}
		else if (cameraDirection_ == XP) {
			cameraChanegeTimer_ += 0.04f;
			// X軸が160を超えないように制限
			if (cameraTranslate.z > 160.0f) {
				cameraTranslate.z = 160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (preCameraTranslate_.x - 140) / (160 - 140)), Lerp(5, 1.3f, (cameraTranslate.z - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (preCameraTranslate_.x - 140) / (160 - 140)), Lerp(-30, -12, (cameraTranslate.z - 140) / (160 - 140)), cameraChanegeTimer_);

			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = ZP;
				cameraChanegeTimer_ = 0.0f;
			}
		}
		if (cameraDirection_ == XM) {
			cameraChanegeTimer_ += 0.02f;
			// X軸が160を超えないように制限
			if (cameraTranslate.z > 160.0f) {
				cameraTranslate.z = 160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), Lerp(5, 1.3f, (cameraTranslate.z - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), Lerp(-30, -12, (cameraTranslate.z - 140) / (160 - 140)), cameraChanegeTimer_);
			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = ZP;
				cameraChanegeTimer_ = 0.0f;
			}
		}
	}
	else if (cameraTranslate.z <= -140.0f) {
		if (cameraDirection_ == NONEDIRECTION) {
			cameraDirection_ = ZM;
		}

		if (cameraDirection_ == ZM) {
			// Z軸が-160を下回らないように制限
			if (cameraTranslate.z < -160.0f) {
				cameraTranslate.z = -160.0f;
			}

			// Z軸の補間に対応するY軸の補間
			float y = Lerp(5, 1.3f, (std::fabs(cameraTranslate.z) - 140) / (160 - 140));
			cameraTranslate.y = y;

			// offsetZ_ の補間（Z軸補間に基づいて）
			offsetZ_ = Lerp(-30, -12, (std::fabs(cameraTranslate.z) - 140) / (160 - 140));
			preCameraTranslate_ = cameraTranslate;
		}
		else if (cameraDirection_ == XP) {
			cameraChanegeTimer_ += 0.02f;
			// Z軸が-160を下回らないように制限
			if (cameraTranslate.z < -160.0f) {
				cameraTranslate.z = -160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (preCameraTranslate_.x - 140) / (160 - 140)), Lerp(5, 1.3f, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (preCameraTranslate_.x - 140) / (160 - 140)), Lerp(-30, -12, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), cameraChanegeTimer_);

			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = ZM;
				cameraChanegeTimer_ = 0.0f;
			}
		}
		if (cameraDirection_ == XM) {
			cameraChanegeTimer_ += 0.02f;
			// X軸が160を超えないように制限
			// Z軸が-160を下回らないように制限
			if (cameraTranslate.z < -160.0f) {
				cameraTranslate.z = -160.0f;
			}
			// X軸の補間に対応するY軸の補間
			float y = Lerp(Lerp(5, 1.3f, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), Lerp(5, 1.3f, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), cameraChanegeTimer_);
			cameraTranslate.y = y;

			// offsetZ_ の補間（X軸補間に基づいて）
			offsetZ_ = Lerp(Lerp(-30, -12, (std::fabs(cameraTranslate.x) - 140) / (160 - 140)), Lerp(-30, -12, (std::fabs(cameraTranslate.z) - 140) / (160 - 140)), cameraChanegeTimer_);
			if (cameraChanegeTimer_ >= 1.0f) {
				cameraDirection_ = ZM;
				cameraChanegeTimer_ = 0.0f;
			}
		}
	}
	else {
		cameraDirection_ = NONEDIRECTION;
		cameraChanegeTimer_ = 0.0f;
	}


	// 中央範囲でリセット
	if (
		cameraTranslate.z > -140.0f && cameraTranslate.z < 140.0f &&
		cameraTranslate.x > -140.0f && cameraTranslate.x < 140.0f) {
		offsetZ_ = -30.0f;
	}
	// カメラの位置設定
	camera_->SetTranslate(cameraTranslate);
}


float FollowCamera::NormalizeAngle(float angle)
{
	float normalizeAngle = angle;
	// -180度から180度の範囲に角度を正規化
	while (normalizeAngle > 1.0f * std::numbers::pi_v<float>) normalizeAngle -= 2.0f * std::numbers::pi_v<float>;
	while (normalizeAngle < -1.0f * std::numbers::pi_v<float>) normalizeAngle += 2.0f * std::numbers::pi_v<float>;
	return normalizeAngle;
}



void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}


