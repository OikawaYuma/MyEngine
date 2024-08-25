#include "Player.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"

void Player::Init()
{
	ModelManager::GetInstance()->LoadModel("Resources/float_Body", "float_Body.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_Head", "float_Head.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_L_arm", "float_L_arm.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_R_arm", "float_R_arm.obj");
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 2.0f;
	worldTransform_.translation_.z = 30.0f;
	worldTransform_2.Initialize();
	worldTransform_2.translation_.y = 1.5f;
	worldTransform_2.parent_ = &worldTransform_;
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("float_Body.obj");

	head_ = std::make_unique<Object3d>();
	head_->Init();
	head_->SetModel("float_Head.obj");

	L_arm_ = std::make_unique<Object3d>();
	L_arm_->Init();
	L_arm_->SetModel("float_L_arm.obj");

	R_arm_ = std::make_unique<Object3d>();
	R_arm_->Init();
	R_arm_->SetModel("float_R_arm.obj");
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/float_Body/tex.png");
}

void Player::Update()
{
	Move();
	worldTransform_.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
	L_arm_->SetWorldTransform(worldTransform_2);
	R_arm_->SetWorldTransform(worldTransform_2);
	head_->SetWorldTransform(worldTransform_2);
}

void Player::Draw()
{
	object_->Draw(skinTex_,camera_);
	L_arm_->Draw(skinTex_,camera_);
	R_arm_->Draw(skinTex_,camera_);
	head_->Draw(skinTex_, camera_);
}

void Player::Move()
{
	Vector3 move{0,0,0};
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
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	

}
