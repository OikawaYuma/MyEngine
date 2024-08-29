#include "Player.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"

#include "ImGuiCommon.h"

void Player::Init()
{
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	//const char* groupName = "Player";
	//// グループを追加
	//GlobalVariables::GetInstance()->CreateGroup(groupName);
	//globalVariables->AddItme(groupName, "main Translation", worldTransform_.translation_);
	//globalVariables->AddItme(groupName, "head Translation",worldTransform_head.translation_);
	
	BaseCharacter::Init();
	ModelManager::GetInstance()->LoadModel("Resources/float_Body", "float_Body.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_Head", "float_Head.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_L_arm", "float_L_arm.obj");
	ModelManager::GetInstance()->LoadModel("Resources/float_R_arm", "float_R_arm.obj");
	ModelManager::GetInstance()->LoadModel("Resources/hammer", "hammer.obj");

	worldTransform_.translation_.y = 1.0f;
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

	worldTransform_weapon.Initialize();
	worldTransform_weapon.translation_.x ;
	worldTransform_weapon.translation_.y;
	worldTransform_weapon.parent_ = &worldTransform_;
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

	weapon_ = std::make_unique<Object3d>();
	weapon_->Init();
	weapon_->SetModel("hammer.obj");
	objects_.push_back(weapon_.get());

	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/float_Body/tex.png");
	InitFloatingGimmmick();
}

void Player::Update()
{
	//ApplyGlobalVariables();
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInit();
		
			break;
		case Behavior::kAttack:
			BehaviorRootAttackInit();
			break;
		case Behavior::kDash:
			BehaviorRootDashInit();
			break;
		case Behavior::kJump:
			BehaviorRootJumpInit();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	ImGui::Begin("ttt");
	ImGui::Text("t%f",worldTransform_.translation_.y);
	ImGui::End();
	switch (behavior_) {
		// 通常行動
	case Behavior::kRoot:
		
	default:
		BehaviorRootUpdate();
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_X)) {
			behaviorRequest_ = Behavior::kAttack;
		}
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y)) {
			behaviorRequest_ = Behavior::kDash;
		}
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_B)) {
			behaviorRequest_ = Behavior::kJump;
		}
		break;
	case Behavior::kAttack:
		BehaviorRootAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorRootDashUpdate();
		break;
	case Behavior::kJump:
		BehaviorRootJumpUpdate();
		break;
	}
	

	BaseCharacter::Update();
	worldTransform_body.UpdateMatrix();
	worldTransform_right.UpdateMatrix();
	worldTransform_left.UpdateMatrix();
	worldTransform_head.UpdateMatrix();
	worldTransform_weapon.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_body);
	L_arm_->SetWorldTransform(worldTransform_left);
	R_arm_->SetWorldTransform(worldTransform_right);
	head_->SetWorldTransform(worldTransform_head);
	weapon_->SetWorldTransform(worldTransform_weapon);
}

void Player::Draw(Camera* camera)
{
	BaseCharacter::Draw(camera);
	
}

void Player::Move()
{
	velo_ = {0,0,0};
	preAngle_ = worldTransform_.rotation_.y;
	if (Input::GetInstance()->GetJoystickState()) {

		velo_.x += Input::GetInstance()->JoyStickParmLX(0.2f);
		velo_.z += Input::GetInstance()->JoyStickParmLY(0.2f);
	}
	if (!(velo_.x == 0 && velo_.y == 0 && velo_.z == 0)) {
		velo_ = Normalize(velo_);
		velo_.x *= 1.4f;
		velo_.y *= 1.4f;
		velo_.z *= 1.4f;
		velo_ = TransformNormal(velo_, camera_->GetCameraMatrix());
		// Y軸周り角度（Θy）
		preAngle_ = std::atan2(velo_.x, velo_.z);
		
	}

	angletime += 0.05f;
	if (1.0f <= angletime ) {
		angletime = 0.0f;
	}
	worldTransform_.rotation_.y = 
		LerpShortAngle(worldTransform_.rotation_.y, preAngle_, angletime);
	worldTransform_.translation_ = Add(worldTransform_.translation_, velo_);

	

}



void Player::InitFloatingGimmmick()
{
	floatingparam_ = 0.0f;
}

void Player::UpdateFloatingGimmmick()
{
	floatingparam_ += step;

	floatingparam_ = std::fmod(floatingparam_, 2.0f * (float)std::numbers::pi);
	const float floathingWidth = 0.05f;
	worldTransform_.translation_.y = worldTransform_.translation_.y  + std::sin(floatingparam_) * floathingWidth;
}

void Player::BehaviorRootUpdate()
{
	Move();
	UpdateFloatingGimmmick();
}

void Player::BehaviorRootAttackUpdate()
{
	worldTransform_weapon.rotation_.x += 0.1f;
	if (((float)std::numbers::pi / 2.0f) <= worldTransform_weapon.rotation_.x) {
		behaviorRequest_ = Behavior::kRoot;
	}

}

void Player::BehaviorRootInit()
{
	worldTransform_weapon.rotation_.x = 0.0f;
}

void Player::BehaviorRootAttackInit()
{
	worldTransform_weapon.rotation_.x = 0.0f;
}

void Player::BehaviorRootDashInit()
{
	workDash_.dashparam_ = 0;
	worldTransform_.rotation_.y = preAngle_;
}

void Player::BehaviorRootDashUpdate()
{
	// ダッシュの時間
	const uint32_t behaviorDashTime = 60;

	velo_ = { 0,0,2 };
	if (!(velo_.x == 0 && velo_.y == 0 && velo_.z == 0)) {
		velo_ = Normalize(velo_);
		velo_.x *= 2.0f;
		velo_.y *= 2.0f;
		velo_.z *= 2.0f;
		velo_ = TransformNormal(velo_, worldTransform_.matWorld_);
		// Y軸周り角度（Θy）
		//preAngle = std::atan2(move.x, move.z);
		worldTransform_.translation_ = Add(worldTransform_.translation_, velo_);

	}

	// 既定の時間経過で通常行動に戻る
	if (++workDash_.dashparam_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorRootJumpInit()
{
	worldTransform_.translation_.y = 2.0;
	worldTransform_right.rotation_.x = 0;
	worldTransform_left.rotation_.x = 0;

	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	// ジャンプ初速を与える
	velo_.y = kJumpFirstSpeed;
}

void Player::BehaviorRootJumpUpdate()
{
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velo_);
	// 重量加速度
	const float kGravityAcceleration = 0.1f;
	// 加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};
	// 加速する
	velo_ = Add(velo_,accelerationVector);

	// 着地
	if (worldTransform_.translation_.y <= 2.0f) {
		worldTransform_.translation_.y = 2.0;
		// ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* gVes = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransform_.translation_ = gVes->GetVector3Value(groupName, "main Translation");
	worldTransform_head.translation_ = gVes->GetVector3Value(groupName, "head Translation");
}
