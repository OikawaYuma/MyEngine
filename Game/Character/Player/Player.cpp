#include "Player.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"
#include "Audio.h"
#include "Object3dManager.h"
#include "ImGuiCommon.h"
#include "LockOn/LockOn.h"

void Player::Init(const Vector3& translate, const std::string filename)
{
#ifdef DEBUG
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItme(groupName, "main Translation", worldTransform_.translation_);
#endif // DEBUG

	
	playerReticleTex_ = TextureManager::GetInstance()->StoreTexture("Resources/Reticle.png");
	playerHpUITex_ = TextureManager::GetInstance()->StoreTexture("Resources/HpUI.png");
	normalBulletUITex_ = TextureManager::GetInstance()->StoreTexture("Resources/bulletUI/NormalBulletUI.png");
	hommingBulletUITex_ = TextureManager::GetInstance()->StoreTexture("Resources/bulletUI/HommingBulletUI.png");
	razerBulletUITex_ = TextureManager::GetInstance()->StoreTexture("Resources/bulletUI/RazerBeam.png");

	hp_ = 1.0f;

	hpUIBlue_ = std::make_unique<Sprite>();
	hpUIBlue_->Init(
		{ (hp_ - 0.3f) * 200.0f  + 50.0f ,25.0f },
		{ (hp_ - 0.3f) * 200.0f, 50.0f },
		{ 1.0f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		"Resources/player.png");
	hpUI_ = std::make_unique<Sprite>();
	hpUI_->Init(
		{ 25.0f ,25.0f },
		{ 50.0f, 50.0f },
		{ 0.5f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		"Resources/HpUI.png");

	bulletModeUI = std::make_unique<Sprite>();
	bulletModeUI->Init(
		{ 1180.0f ,620.0f },
		{ 100.0f, 100.0f },
		{ 0.5f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		"Resources/bulletUI/NormalBulletUI.png");

	bulletSize_ = hp_ / 2;
	worldTransform3DReticleNear_.Initialize();
	worldTransform3DReticleNear_.scale_ = { bulletSize_,bulletSize_,bulletSize_ };
	worldTransform3DReticleFar_.Initialize();
	worldTransform3DReticleFar_.scale_ = {0.5f,0.5f,0.5f };
	
	ModelManager::GetInstance()->LoadModel("Resources/player", "player.obj");
	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	ModelManager::GetInstance()->LoadModel("Resources/player", "Reticle3.obj");
	ModelManager::GetInstance()->LoadModel("Resources/player", "Reticle4.obj");

	worldTransform_.Initialize();
	worldTransform_.translation_ = translate;
	worldTransform_.scale_ = { hp_ ,hp_ ,hp_ };
	worldTransform_.translation_.y = worldTransform_.scale_.y;
	
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/player/player.png");

	reticleColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//Object3dManager::GetInstance()->StoreObject("Reticle3", &worldTransform3DReticleNear_, skinTex_, &reticleColor_);



	
	InitFloatingGimmmick();
	SetCollisonAttribute(0b0001);
	SetCollisionMask(0b0110);


	// 色のデータを変数から読み込み
	coolTimeAlpha_ = 0.8f;
	material_.color = { 1.0f,1.0f,1.0f,coolTimeAlpha_ };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;
	Object3dManager::GetInstance()->StoreObject(filename,&worldTransform_,skinTex_,&material_.color, Transparency::Transparent);
	spotLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	spotLight_.position = worldTransform_.translation_;
	spotLight_.distance = 7.0f;
	spotLight_.direction =
		Normalize(Vector3{ -1.0f,-1.0f,0.0f });
	spotLight_.intensity = 4.0f;
	spotLight_.dacya = 2.0f;
	spotLight_.cosAngle =
		std::cos(std::numbers::pi_v<float> / 3.0f);


	direLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	direLight_.direction = { 0.0f,-1.0f,0.0f };
	direLight_.intensity = 0.6f;
	//object_->SetDirectionLight(direLight_);



	worldTransform_.UpdateMatrix();

	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&worldTransform_, "player.obj");
	shadowObject_->Update();
	Object3dManager::GetInstance()->StoreObject(filename, shadowObject_->GetWorldTransform(), skinTex_, shadowObject_->GetColor(), Transparency::Transparent);
	reticleShadowObject_ = std::make_unique<PlaneProjectionShadow>();
	reticleShadowObject_->Init(&worldTransform3DReticleNear_, "Reticle4.obj");
	reticleShadowObject_->Update();
	Object3dManager::GetInstance()->StoreObject(filename, reticleShadowObject_->GetWorldTransform(), skinTex_, reticleShadowObject_->GetColor(),Transparency::Transparent);
	reticleY_ = 0;

	////////////////////////////////////////////
	// SE
	/////////////////////////////////////////////
	//jumpSE_ = Audio::GetInstance()->SoundLoadWave("Resources/jump3.wav");
	bulletShotSE_ = Audio::GetInstance()->SoundLoadWave("Resources/bullet.wav");

	guideUI_ = std::make_unique<GuideUI>();
	guideUI_->Init();
	UIColor_ = { 1.0f,1.0f,1.0f,1.0f };


}

void Player::Update()
{
	ColorAdust();
#ifdef Debug
	
#endif // DEBUG

	/*ImGui::Begin("Light");
	ImGui::DragFloat4("mColor", &material_.color.x, 0.1f);
	ImGui::DragFloat("mShin", &material_.shininess, 0.1f);

	ImGui::DragFloat4("dColor", &direLight_.color.x, 0.1f);
	ImGui::DragFloat3("ddire", &direLight_.direction.x, 0.1f);
	ImGui::DragFloat("dinten", &direLight_.intensity, 0.1f);

	ImGui::DragFloat4("sColor", &spotLight_.color.x, 0.1f);
	ImGui::DragFloat3("sDire", &spotLight_.direction.x, 0.1f);
	ImGui::DragFloat3("sPos", &spotLight_.position.x, 0.1f);
	ImGui::DragFloat3("sDis", &spotLight_.distance, 0.1f);
	ImGui::DragFloat("sInten", &spotLight_.intensity, 0.1f);
	ImGui::DragFloat("sDacya", &spotLight_.dacya, 0.1f);

	ImGui::Text("playerPosX %f", worldTransform_.translation_.x);
	ImGui::Text("playerPosZ %f", worldTransform_.translation_.z);
	ImGui::End();*/


	direLight_.direction = Normalize(direLight_.direction);
	spotLight_.direction = Normalize(spotLight_.direction);

	//object_->SetMaterial(material_);
	//object_->SetSpotLight(spotLight_);
	//object_->SetDirectionLight(direLight_);


	hpUIBlue_->SetPosition({ (hp_ - 0.3f) * 200 + 50.0f,25.0f });
	hpUIBlue_->SetSize({ (hp_ - 0.3f) * 200,50.0f });
	hpUIBlue_->Update();

	HitEnemyCoolTime();

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
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_,hp_,hp_ };
	bulletSize_ = hp_ / 2;
	worldTransform3DReticleNear_.scale_ = { bulletSize_,bulletSize_,bulletSize_ };
	// 着地
	if (worldTransform_.translation_.y <= (hp_)) {
		worldTransform_.translation_.y = (hp_);
		// ジャンプ終了
		//behaviorRequest_ = Behavior::kRoot;
	}
	//SetRadius(hp_);
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	razers_.remove_if([](PlayerRazer* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});


	

	switch (behavior_) {
		// 通常行動
	case Behavior::kRoot:
		
	default:
		BehaviorRootUpdate();
		
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
	
	
	UIColor_ = { 1.0f,1.0f,1.0f,1.0f };
	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_X)) {
		UIColor_ = { 0.5f,0.5f,0.5f,1.0f };
		switch (bulletMode_) {
		case BulletMode::NormalBullet:
			bulletMode_ = HommingBullet;
			break;
		case BulletMode::HommingBullet:
			bulletMode_ = LaserBeam;
			break;
		case BulletMode::LaserBeam:
			bulletMode_ = NormalBullet;
			break;
		}
	}
	else if (Input::GetInstance()->TriggerKey(DIK_K)) {
		
		switch (bulletMode_) {
		case BulletMode::NormalBullet:
			bulletMode_ = HommingBullet;
			break;
		case BulletMode::HommingBullet:
			bulletMode_ = LaserBeam;
			break;
		case BulletMode::LaserBeam:
			bulletMode_ = NormalBullet;
			break;
		}
	}



	Aim();
	Attack();
	// 弾更新
	for (std::list<PlayerBullet*>::iterator itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		(*itr)->Update();
	}
	// 弾更新
	for (std::list<PlayerRazer*>::iterator itr = razers_.begin(); itr != razers_.end(); itr++) {
		(*itr)->Update();
	}

	if (worldTransform_.translation_.x >= movingRange_) {
		worldTransform_.translation_.x = movingRange_;
	}
	else if (worldTransform_.translation_.x <= -movingRange_) {
		worldTransform_.translation_.x = -movingRange_;
	}
	if (worldTransform_.translation_.z <= -movingRange_) {
		worldTransform_.translation_.z = -movingRange_;
	}
	else if (worldTransform_.translation_.z >= movingRange_) {
		worldTransform_.translation_.z = movingRange_;
	}

	worldTransform_.UpdateMatrix();

	guideUI_->Update();
	shadowObject_->Update();
	reticleShadowObject_->Update();
}

void Player::Draw(Camera* camera)
{
	//shadowObject_->Draw(camera);
	//reticleShadowObject_->Draw(camera);
	for (std::list<PlayerBullet*>::iterator itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		(*itr)->Draw(camera);
	}
	for (std::list<PlayerRazer*>::iterator itr = razers_.begin(); itr != razers_.end(); itr++) {
		(*itr)->Draw(camera);
	}
	
	switch (bulletMode_) {
	case BulletMode::NormalBullet:
		break;
	case BulletMode::HommingBullet:
		break;
	case BulletMode::LaserBeam:
		break;
	}



}

void Player::DrawUI()
{
	
	hpUI_->Draw(playerHpUITex_, { 1.0f,1.0f,1.0f,1.0f });
	hpUIBlue_->Draw(skinTex_, { 1.0f,1.0f,1.0f,1.0f });
	guideUI_->Draw();
	switch (bulletMode_) {
	case BulletMode::NormalBullet:
		bulletModeUI->Draw(normalBulletUITex_, UIColor_);
		break;
	case BulletMode::HommingBullet:
		bulletModeUI->Draw(hommingBulletUITex_, UIColor_);
		break;
	case BulletMode::LaserBeam:
		bulletModeUI->Draw(razerBulletUITex_, UIColor_);
		break;
	}
}

void Player::TitleInit()
{
#ifdef DEBUG
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItme(groupName, "main Translation", worldTransform_.translation_);
#endif // DEBUG




	ModelManager::GetInstance()->LoadModel("Resources/player", "player.obj");

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,1.0f,100.0f };
	worldTransform_.translation_.y = 0.5f;


	color_ = {1.0f,1.0f,1.0f,0.8f,};
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/player/player.png");
	Object3dManager::GetInstance()->StoreObject("player", &worldTransform_, skinTex_, &color_, Transparency::Transparent);
	//BehaviorRootJumpInit();
	
	InitFloatingGimmmick();
	SetCollisonAttribute(0b0001);
	SetCollisionMask(0b0110);
	behaviorRequest_ = Behavior::kTitlePlayer;
}

void Player::ClearInit()
{
	ModelManager::GetInstance()->LoadModel("Resources/player", "player.obj");

	worldTransform_.Initialize();
	worldTransform_.translation_ = { -2.838254928588867f,1.0f,231.0f };
	worldTransform_.translation_.y = 0.5f;

	coolTimeAlpha_ = 0.8f;
	material_.color = { 1.0f,1.0f,1.0f,coolTimeAlpha_ };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;
	color_ = { 1.0f,1.0f,1.0f,1.0f, };
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/player/player.png");
	Object3dManager::GetInstance()->StoreObject("player", &worldTransform_, skinTex_, &material_.color, Transparency::Transparent);

	//BehaviorRootJumpInit();
	InitFloatingGimmmick();
	SetCollisonAttribute(0b0001);
	SetCollisionMask(0b0110);
	behaviorRequest_ = Behavior::kTitlePlayer;
}

void Player::ClearUpdate()
{
	// 重量加速度
	const float kGravityAcceleration = 0.075f;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0, -kGravityAcceleration, 0 };
	// 加速する
	velo_ = Add(velo_, accelerationVector);
	

	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_ + 0.3f,hp_ + 0.3f,hp_ + 0.3f };
	// 着地
	if (worldTransform_.translation_.y <= (hp_ + 0.3f)) {
		worldTransform_.translation_.y = (hp_ + 0.3f);
		// ジャンプ初速
		const float kJumpFirstSpeed = 1.0f;
		// ジャンプ初速を与える
		velo_.y = kJumpFirstSpeed;
	}

	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, { 0.0f,velo_.y,0.0f });

	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void Player::TitleUpdate()
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
		case Behavior::kTitlePlayer:
			BehaviorRootJumpInit();
			break;
		}
		
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_,hp_,hp_ };
	//SetRadius(hp_);
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	razers_.remove_if([](PlayerRazer* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});


	//Aim();
	
	switch (behavior_) {
		// 通常行動
	case Behavior::kRoot:

	default:
		BehaviorRootUpdate();
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_X)) {
			//behaviorRequest_ = Behavior::kAttack;
		}
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y)) {
			//behaviorRequest_ = Behavior::kDash;
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
	case Behavior::kTitlePlayer:
		BehaviorRootTitleJumpUpdate();
		break;
	}



	if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_RIGHT_THUMB)) {
		switch (bulletMode_) {
		case BulletMode::NormalBullet:
			bulletMode_ = HommingBullet;
			break;
		case BulletMode::HommingBullet:
			bulletMode_ = LaserBeam;
			break;
		case BulletMode::LaserBeam:
			bulletMode_ = NormalBullet;
			break;
		}
	}

	else if (Input::GetInstance()->TriggerKey(DIK_K)) {

		switch (bulletMode_) {
		case BulletMode::NormalBullet:
			bulletMode_ = HommingBullet;
			break;
		case BulletMode::HommingBullet:
			bulletMode_ = LaserBeam;
			break;
		case BulletMode::LaserBeam:
			bulletMode_ = NormalBullet;
			break;
		}
	}




	//BehaviorRootJumpUpdate();
	worldTransform_.translation_.z += 0.25f;
	Attack();
	// 弾更新
	for (std::list<PlayerBullet*>::iterator itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		(*itr)->Update();
	}
	// 弾更新
	for (std::list<PlayerRazer*>::iterator itr = razers_.begin(); itr != razers_.end(); itr++) {
		(*itr)->Update();
	}
	worldTransform_.UpdateMatrix();
	shadowObject_->Update();
}

void Player::GameOverInit()
{
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/slimeDead/slimeDead.png");

	ModelManager::GetInstance()->LoadModel("Resources/slimeDead", "slimeDead.obj");
	deadSlimeObj_ = std::make_unique<Object3d>();
	deadSlimeObj_->Init();
	deadSlimeObj_->SetModel("slimeDead.obj");
	deadSlimeObj_->SetSkinTex(skinTex_);
	worldTransform_.Initialize();
	worldTransform_.translation_ = {-1.0f,0.01f,221.0f };
	worldTransform_.scale_ = { 5.0f,1.0f,5.0f };
	worldTransform_.UpdateMatrix();
	deadSlimeObj_->SetWorldTransform(worldTransform_);

}

void Player::GameOverUpdate()
{
	worldTransform_.UpdateMatrix();
	deadSlimeObj_->SetWorldTransform(worldTransform_);
	deadSlimeObj_->Update();
}

void Player::GameOverDraw(Camera* camera)
{
	deadSlimeObj_->Draw(camera);
}

void Player::DemoUpdate()
{
	ColorAdust();
#ifdef Debug

#endif // DEBUG

	/*ImGui::Begin("Light");
	ImGui::DragFloat4("mColor", &material_.color.x, 0.1f);
	ImGui::DragFloat("mShin", &material_.shininess, 0.1f);

	ImGui::DragFloat4("dColor", &direLight_.color.x, 0.1f);
	ImGui::DragFloat3("ddire", &direLight_.direction.x, 0.1f);
	ImGui::DragFloat("dinten", &direLight_.intensity, 0.1f);

	ImGui::DragFloat4("sColor", &spotLight_.color.x, 0.1f);
	ImGui::DragFloat3("sDire", &spotLight_.direction.x, 0.1f);
	ImGui::DragFloat3("sPos", &spotLight_.position.x, 0.1f);
	ImGui::DragFloat3("sDis", &spotLight_.distance, 0.1f);
	ImGui::DragFloat("sInten", &spotLight_.intensity, 0.1f);
	ImGui::DragFloat("sDacya", &spotLight_.dacya, 0.1f);

	ImGui::Text("playerPosX %f", worldTransform_.translation_.x);
	ImGui::Text("playerPosZ %f", worldTransform_.translation_.z);
	ImGui::End();*/


	direLight_.direction = Normalize(direLight_.direction);
	spotLight_.direction = Normalize(spotLight_.direction);

	hpUIBlue_->SetPosition({ hp_ * 200 + 50.0f,25.0f });
	hpUIBlue_->SetSize({ hp_ * 200,50.0f });
	hpUIBlue_->Update();

	HitEnemyCoolTime();

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
	// HPを元に基準となる大きさを決定する
	worldTransform_.scale_ = { hp_ + 0.3f,hp_ + 0.3f,hp_ + 0.3f };
	// 着地
	if (worldTransform_.translation_.y <= (hp_ + 0.3f)) {
		worldTransform_.translation_.y = (hp_ + 0.3f);
		// ジャンプ終了
		//behaviorRequest_ = Behavior::kRoot;
	}

	if (hp_ <= 0.3) {
		hp_ = 1.0f;
	}

	Aim();

	switch (behavior_) {
		// 通常行動
	case Behavior::kRoot:

	default:
		/*BehaviorRootUpdate();

		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y)) {
			behaviorRequest_ = Behavior::kDash;
		}
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_B)) {
			behaviorRequest_ = Behavior::kJump;
		}*/
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

	worldTransform_.UpdateMatrix();


	shadowObject_->Update();
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
		accel_ += 0.05f;
		if (accel_ >=1.4f ) {
			accel_ = 1.4f;
		}
		velo_ = Normalize(velo_);
		velo_.x *= accel_;
		velo_.y *= accel_;
		velo_.z *= accel_;
		velo_ = TransformNormal(velo_, camera_->GetCameraMatrix());
		// Y軸周り角度（Θy）
		preAngle_ = std::atan2(velo_.x, velo_.z);
		
	}
	else if ((velo_.x == 0 && velo_.y == 0 && velo_.z == 0)) {
		accel_ = 0.0f;
	}
	


	angletime += 0.05f;
	if (1.0f <= angletime ) {
		angletime = 0.0f;
	}
	worldTransform_.rotation_.y = 
		LerpShortAngle(worldTransform_.rotation_.y, preAngle_, angletime);
	worldTransform_.translation_ = Add(worldTransform_.translation_, velo_);

	worldTransform_.UpdateMatrix();

}

void Player::Aim()
{
	Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A);
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState()) {
		//spritePosition.x += Input::GetInstance()->JoyStickParmRX(12.0f);
		reticleY_ += Input::GetInstance()->JoyStickParmRY(0.5f);
		if (reticleY_ <= bulletSize_) {
			reticleY_ = bulletSize_;
		}
		else if (reticleY_ >= 15.0f) {
			reticleY_ = 15.0f;
		}
	}

	const float kDistanceTextObjectNear = 30.0f;
	const float kDistanceTextObjectFar = 20.0f;
	
	cameraDirection_ = { 0.0f,0.0f,1.0f };
	cameraDirection_ = TransformNormal(cameraDirection_, camera_->GetCameraMatrix());
	cameraDirection_.y = 0.0f;
	worldTransform3DReticleNear_.translation_.x = worldTransform_.translation_.x + cameraDirection_.x * kDistanceTextObjectNear;
	worldTransform3DReticleNear_.translation_.y = worldTransform_.translation_.y + reticleY_;
	worldTransform3DReticleNear_.translation_.z = worldTransform_.translation_.z + cameraDirection_.z * kDistanceTextObjectNear;
	worldTransform3DReticleNear_.UpdateMatrix();

	worldTransform3DReticleFar_.translation_.x = GetReticleWorldPosition().x - GetWorldPosition().x;
	worldTransform3DReticleFar_.translation_.y = GetReticleWorldPosition().y - GetWorldPosition().y;
	worldTransform3DReticleFar_.translation_.z = GetReticleWorldPosition().z - GetWorldPosition().z;
	worldTransform3DReticleFar_.translation_ = Normalize(worldTransform3DReticleFar_.translation_);

	worldTransform3DReticleFar_.translation_.x = GetWorldPosition().x + worldTransform3DReticleFar_.translation_.x * kDistanceTextObjectFar;
	worldTransform3DReticleFar_.translation_.y = GetWorldPosition().y + worldTransform3DReticleFar_.translation_.y * kDistanceTextObjectFar;
	worldTransform3DReticleFar_.translation_.z = GetWorldPosition().z + worldTransform3DReticleFar_.translation_.z * kDistanceTextObjectFar;
	worldTransform3DReticleFar_.UpdateMatrix();

	Vector3 velocity(0, 0, 1.0f);
	// 自機から照準オブジェクトへのベクトル
	velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
	velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
	velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;

	velocity = Normalize(velocity);
	// Y軸周り角度（Θy）
	worldTransform3DReticleFar_.rotation_.y = std::atan2(velocity.x, velocity.z);
	worldTransform3DReticleNear_.rotation_.y = std::atan2(velocity.x, velocity.z);
	float velocityXZ = sqrt((velocity.x * velocity.x) + (velocity.z * velocity.z));

	worldTransform3DReticleFar_.rotation_.x = std::atan2(-velocity.y, velocityXZ);
	worldTransform3DReticleNear_.rotation_.x = std::atan2(-velocity.y, velocityXZ);


}

void Player::Attack()
{
	shotTimer_++;
	switch (bulletMode_) {
	case BulletMode::NormalBullet:

		if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) && shotInterval_ <= shotTimer_) {
			Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), bulletShotSE_, false);
			shotTimer_ = 0;
			// 自キャラの座標をコピー
			Vector3 position = {
				worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
				worldTransform_.matWorld_.m[3][2] 
			};

			// 弾の速度
			const float kBulletSpeed = 3.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			// 自機から照準オブジェクトへのベクトル
			velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
			velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
			velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;



			velocity = Normalize(velocity);
			velocity.x *= kBulletSpeed;
			velocity.y *= kBulletSpeed;
			velocity.z *= kBulletSpeed;;

			hp_ -= 0.01f;
			worldTransform_.translation_.y -= 0.01f;
			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Init(GetWorldPosition(), velocity, bulletSize_);
			// 弾を登録する
			bullets_.push_back(newBullet);
		}

		else if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

			// 自キャラの座標をコピー
			Vector3 position = {
				worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
				worldTransform_.matWorld_.m[3][2] };

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			// 自機から照準オブジェクトへのベクトル
			velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
			velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
			velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;



			velocity = Normalize(velocity);
			velocity.x *= kBulletSpeed;
			velocity.y *= kBulletSpeed;
			velocity.z *= kBulletSpeed;;

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Init(GetWorldPosition(), velocity, bulletSize_);
			// 弾を登録する
			bullets_.push_back(newBullet);

		}
		break;
	case BulletMode::HommingBullet:
		if (lockOn_->GetTarget()) {
			if (Input::GetInstance()->PushJoyButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) && shotInterval_ <= shotTimer_) {
				Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), bulletShotSE_, false);
				shotTimer_ = 0;
				// 自キャラの座標をコピー
				Vector3 position = {
					worldTransform_.matWorld_.m[3][0],
					worldTransform_.matWorld_.m[3][1],
					worldTransform_.matWorld_.m[3][2] };

				// 弾の速度
				const float kBulletSpeed = 3.0f;
				Vector3 velocity(0, 0, kBulletSpeed);
				// 自機から照準オブジェクトへのベクトル
				velocity.x = lockOn_->GetTarget()->GetWorldPosition().x - GetWorldPosition().x;
				velocity.y = lockOn_->GetTarget()->GetWorldPosition().y - GetWorldPosition().y;
				velocity.z = lockOn_->GetTarget()->GetWorldPosition().z - GetWorldPosition().z;



				velocity = Normalize(velocity);
				velocity.x *= kBulletSpeed;
				velocity.y *= kBulletSpeed;
				velocity.z *= kBulletSpeed;;

				hp_ -= 0.01f;
				worldTransform_.translation_.y -= 0.01f;

				PlayerBullet* newBullet = new PlayerBullet();
				newBullet->Init(GetWorldPosition(), velocity, bulletSize_);
				//newBullet->SetParent(worldTransform_.parent_);
				// 弾を登録する
				bullets_.push_back(newBullet);
			}

			else if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

				// 自キャラの座標をコピー
				Vector3 position = {
					worldTransform_.matWorld_.m[3][0],
					worldTransform_.matWorld_.m[3][1],
					worldTransform_.matWorld_.m[3][2] };

				// 弾の速度
				const float kBulletSpeed = 1.0f;
				Vector3 velocity(0, 0, kBulletSpeed);
				// 自機から照準オブジェクトへのベクトル
				velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
				velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
				velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;



				velocity = Normalize(velocity);
				velocity.x *= kBulletSpeed;
				velocity.y *= kBulletSpeed;
				velocity.z *= kBulletSpeed;;

				// 速度ベクトルを自機の向きに合わせて回転させる
				//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

				//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
				// 弾を生成し、初期化
				PlayerBullet* newBullet = new PlayerBullet();
				newBullet->Init(GetWorldPosition(), velocity, bulletSize_);
				//newBullet->SetParent(worldTransform_.parent_);
				// 弾を登録する
				bullets_.push_back(newBullet);

			}
		}
		break;

	case BulletMode::LaserBeam:

		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			// 自キャラの座標をコピー
			Vector3 position = {
				worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
				worldTransform_.matWorld_.m[3][2] };

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			// 自機から照準オブジェクトへのベクトル
			velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
			velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
			velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;



			velocity = Normalize(velocity);
			velocity.x *= kBulletSpeed;
			velocity.y *= kBulletSpeed;
			velocity.z *= kBulletSpeed;;

			// 速度ベクトルを自機の向きに合わせて回転させる
			//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
			PlayerRazer* newBullet = new PlayerRazer();
			newBullet->Init(GetWorldPosition(), velocity);
			//newBullet->SetParent(&worldTransform_);
			//newBullet->SetParent(worldTransform_.parent_);
			// 弾を登録する
			razers_.push_back(newBullet);
		}

		else if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

			// 自キャラの座標をコピー
			Vector3 position = {
				worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
				worldTransform_.matWorld_.m[3][2] };

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			// 自機から照準オブジェクトへのベクトル
			velocity.x = GetReticleWorldPosition().x - GetWorldPosition().x;
			velocity.y = GetReticleWorldPosition().y - GetWorldPosition().y;
			velocity.z = GetReticleWorldPosition().z - GetWorldPosition().z;



			velocity = Normalize(velocity);
			velocity.x *= kBulletSpeed;
			velocity.y *= kBulletSpeed;
			velocity.z *= kBulletSpeed;;

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
			// 弾を生成し、初期化
			PlayerRazer* newBullet = new PlayerRazer();
			newBullet->Init(GetWorldPosition(), velocity);
			//newBullet->SetParent(worldTransform_.parent_);
			// 弾を登録する
			razers_.push_back(newBullet);

			break;
		}
	}
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

void Player::HitEnemyCoolTime()
{
	if (isEnemyHit_) {
		coolTimer_++;
		// 透明度増減
		coolTimeAlpha_ -= coolTimeAlphaPorM_;
		if (coolTimeAlpha_ <= 0.0f) {
			coolTimeAlphaPorM_ *= -1.0f;
		}
		else if (coolTimeAlpha_ >= slimeBasedAlpha_) {
			coolTimeAlphaPorM_ *= -1.0f;
		}


		if (coolTimer_ >= 120) {
			isEnemyHit_ = false;
			coolTimer_ = 0;
		}
		material_.color = { 1.0f, 1.0f, 1.0f, coolTimeAlpha_ };
	}
	else {
		material_.color = { 1.0f, 1.0f, 1.0f, slimeBasedAlpha_ };
	}

}

void Player::HitEnemySlime()
{
	if (!isEnemyHit_) {
		hp_ -= 0.1f;
		worldTransform_.translation_.y -= 0.1f;
		isEnemyHit_ = true;
	}
}

void Player::CreateReticle()
{
	Object3dManager::GetInstance()->StoreObject("Reticle3", &worldTransform3DReticleNear_, skinTex_, &reticleColor_, Transparency::Transparent);
}

void Player::BehaviorRootUpdate()
{
	Move();
	//UpdateFloatingGimmmick();
}

void Player::BehaviorRootAttackUpdate()
{
}

void Player::BehaviorRootInit()
{
}

void Player::BehaviorRootAttackInit()
{
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
	if (worldTransform_.translation_.y <= (hp_)) {
		worldTransform_.translation_.y = (hp_);
		// ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorRootTitleJumpUpdate()
{
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velo_);
	// 重量加速度
	const float kGravityAcceleration = 0.1f;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0, -kGravityAcceleration, 0 };
	// 加速する
	velo_ = Add(velo_, accelerationVector);

	// 着地
	if (worldTransform_.translation_.y <= 0.5f * hp_) {
		worldTransform_.translation_.y = 0.5f * hp_;
		// タイトル用なので無限にジャンプする
		BehaviorRootJumpInit();
	}
}

void Player::ApplyGlobalVariables()
{
#ifdef DEBUG
	GlobalVariables* gVes = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransform_.translation_ = gVes->GetVector3Value(groupName, "main Translation");
#endif // DEBUG


}

void Player::ColorAdust()
{
	
	/*ImGui::Begin("calor");
	ImGui::DragFloat4("color : ", &material_.color.x,0.01f);
	ImGui::End();*/
	//object_->SetMaterial({ .color = material_.color });
}

Vector3 Player::GetReticleWorldPosition()
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticleNear_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticleNear_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticleNear_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision(uint32_t attri)
{
	if (attri) {

	}
}

Vector3 Player::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
