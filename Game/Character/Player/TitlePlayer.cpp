#include "TitlePlayer.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"

#include "ImGuiCommon.h"
#include "LockOn/LockOn.h"

void TitlePlayer::Init()
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
	worldTransform_.translation_ = {0.0f,1.0f,30.0f};
	worldTransform_.translation_.y = 0.5f;


	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetModel("player.obj");

	

	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/player/player.png");
	InitFloatingGimmmick();
	SetCollisonAttribute(0b0001);
	SetCollisionMask(0b0110);
}

void TitlePlayer::Update()
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


	Aim();

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
	object_->SetWorldTransform(worldTransform_);
	object_->Update();
}

void TitlePlayer::Draw(Camera* camera)
{
	object_->Draw(skinTex_, camera);
	
	for (std::list<PlayerBullet*>::iterator itr = bullets_.begin(); itr != bullets_.end(); itr++) {
		(*itr)->Draw(camera);
	}
	for (std::list<PlayerRazer*>::iterator itr = razers_.begin(); itr != razers_.end(); itr++) {
		(*itr)->Draw(camera);
	}

}

void TitlePlayer::DrawUI()
{
	

}

void TitlePlayer::Move()
{
	velo_ = { 0,0,0 };
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
	if (1.0f <= angletime) {
		angletime = 0.0f;
	}
	worldTransform_.rotation_.y =
		LerpShortAngle(worldTransform_.rotation_.y, preAngle_, angletime);
	worldTransform_.translation_ = Add(worldTransform_.translation_, velo_);



}

void TitlePlayer::Aim()
{
	
}



void TitlePlayer::Attack()
{
	switch (bulletMode_) {
	case BulletMode::NormalBullet:
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			// 自キャラの座標をコピー
			Vector3 position = {
				worldTransform_.matWorld_.m[3][0],
				worldTransform_.matWorld_.m[3][1],
				worldTransform_.matWorld_.m[3][2]
			};

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

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Init(GetWorldPosition(), velocity);
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
			newBullet->Init(GetWorldPosition(), velocity);
			// 弾を登録する
			bullets_.push_back(newBullet);

		}
		break;
	case BulletMode::HommingBullet:
		if (lockOn_->GetTarget()) {
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
				velocity.x = lockOn_->GetTarget()->GetWorldPosition().x - GetWorldPosition().x;
				velocity.y = lockOn_->GetTarget()->GetWorldPosition().y - GetWorldPosition().y;
				velocity.z = lockOn_->GetTarget()->GetWorldPosition().z - GetWorldPosition().z;



				velocity = Normalize(velocity);
				velocity.x *= kBulletSpeed;
				velocity.y *= kBulletSpeed;
				velocity.z *= kBulletSpeed;;

				// 速度ベクトルを自機の向きに合わせて回転させる
				//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

				//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
				// 弾を生成し、初期化
				PlayerBullet* newBullet = new PlayerBullet();
				newBullet->Init(GetWorldPosition(), velocity);
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
				newBullet->Init(GetWorldPosition(), velocity);
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

void TitlePlayer::InitFloatingGimmmick()
{
	floatingparam_ = 0.0f;
}

void TitlePlayer::UpdateFloatingGimmmick()
{
	floatingparam_ += step;

	floatingparam_ = std::fmod(floatingparam_, 2.0f * (float)std::numbers::pi);
	const float floathingWidth = 0.05f;
	worldTransform_.translation_.y = worldTransform_.translation_.y + std::sin(floatingparam_) * floathingWidth;
}

void TitlePlayer::BehaviorRootUpdate()
{
	Move();
	//UpdateFloatingGimmmick();
}

void TitlePlayer::BehaviorRootAttackUpdate()
{
}

void TitlePlayer::BehaviorRootInit()
{
}

void TitlePlayer::BehaviorRootAttackInit()
{
}

void TitlePlayer::BehaviorRootDashInit()
{
	workDash_.dashparam_ = 0;
	worldTransform_.rotation_.y = preAngle_;
}

void TitlePlayer::BehaviorRootDashUpdate()
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

void TitlePlayer::BehaviorRootJumpInit()
{
	worldTransform_.translation_.y = 2.0;

	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	// ジャンプ初速を与える
	velo_.y = kJumpFirstSpeed;
}

void TitlePlayer::BehaviorRootJumpUpdate()
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
	if (worldTransform_.translation_.y <= 0.5f) {
		worldTransform_.translation_.y = 0.5f;
		// ジャンプ終了
		BehaviorRootJumpInit();
	}
}

void TitlePlayer::ApplyGlobalVariables()
{
#ifdef DEBUG
	GlobalVariables* gVes = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransform_.translation_ = gVes->GetVector3Value(groupName, "main Translation");
#endif // DEBUG


}

Vector3 TitlePlayer::GetReticleWorldPosition()
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticleNear_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticleNear_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticleNear_.matWorld_.m[3][2];

	return worldPos;
}

void TitlePlayer::OnCollision(uint32_t attri)
{
}

Vector3 TitlePlayer::GetWorldPosition() const
{
	// ワールド行列座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

