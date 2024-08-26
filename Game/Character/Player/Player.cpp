#include "Player.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"

void Player::Init()
{
	BaseCharacter::Init();
	ModelManager::GetInstance()->LoadModel("Resources/float_Body", "float_Body.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_Head", "float_Head.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_L_arm", "float_L_arm.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_R_arm", "float_R_arm.obj");

	worldTransform_.translation_.y = 3.0f;
	worldTransform_.translation_.z = 30.0f;

	worldTransform_body.Initialize();
	worldTransform_body.translation_.y = 0;
	worldTransform_body.parent_ = &worldTransform_;

	worldTransform_head.Initialize();
	worldTransform_head.translation_.y = 1.5f;
	worldTransform_head.parent_ = &worldTransform_;

	worldTransform_right.Initialize();
	worldTransform_right.translation_.x = 2.0f;
	worldTransform_right.translation_.y = 1.5f;
	worldTransform_right.parent_ = &worldTransform_;

	worldTransform_left.Initialize();
	worldTransform_left.translation_.x = -2.0f;
	worldTransform_left.translation_.y = 1.5f;
	worldTransform_left.parent_ = &worldTransform_;
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("float_Body.obj");
	objects_.push_back(object_.get());

	head_ = std::make_unique<Object3d>();
	head_->Init();
	head_->SetModel("float_Head.obj");
	objects_.push_back(head_.get());

	L_arm_ = std::make_unique<Object3d>();
	L_arm_->Init();
	L_arm_->SetModel("float_L_arm.obj");
	objects_.push_back(L_arm_.get());

	R_arm_ = std::make_unique<Object3d>();
	R_arm_->Init();
	R_arm_->SetModel("float_R_arm.obj");
	objects_.push_back(R_arm_.get());

	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/float_Body/tex.png");
	InitFloatingGimmmick();
}

void Player::Update()
{
	BaseCharacter::Update();
	Move();
	UpdateFloatingGimmmick();
	worldTransform_.UpdateMatrix();
	worldTransform_body.UpdateMatrix();
	worldTransform_right.UpdateMatrix();
	worldTransform_left.UpdateMatrix();
	worldTransform_head.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_body);
	L_arm_->SetWorldTransform(worldTransform_left);
	R_arm_->SetWorldTransform(worldTransform_right);
	head_->SetWorldTransform(worldTransform_head);
}

void Player::Draw(Camera* camera)
{
	BaseCharacter::Draw(camera);
	
}

void Player::Move()
{
	Vector3 move{0,0,0};
	float preAngle = worldTransform_.rotation_.y;
	if (Input::GetInstance()->GetJoystickState()) {

		move.x += Input::GetInstance()->JoyStickParmLX(0.2f);
		move.z += Input::GetInstance()->JoyStickParmLY(0.2f);
	}
	if (!(move.x == 0 && move.y == 0 && move.z == 0)) {
		move = Normalize(move);
		move.x *= 1.4f;
		move.y *= 1.4f;
		move.z *= 1.4f;
		move = TransformNormal(move, camera_->GetCameraMatrix());
		// Y軸周り角度（Θy）
		preAngle = std::atan2(move.x, move.z);
		
	}

	angletime += 0.05f;
	if (1.0f <= angletime ) {
		angletime = 0.0f;
	}
	worldTransform_.rotation_.y = 
		LerpShortAngle(worldTransform_.rotation_.y, preAngle, angletime);
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	

}

void Player::InitFloatingGimmmick()
{
	floatingparam_ = 0.0f;
}

void Player::UpdateFloatingGimmmick()
{
	floatingparam_ += step;

	floatingparam_ = std::fmod(floatingparam_, 2.0f * (float)std::numbers::pi);
	const float floathingWidth = 0.5f;
	worldTransform_.translation_.y = std::sin(floatingparam_) * floathingWidth;
}
