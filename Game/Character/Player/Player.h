/**
* @ file
* @ brief
*/

#pragma once
#include <memory>
#include <optional>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"
#include "Character/base/BaseCharacter.h"
#pragma warning(push)
// C26819のエラーを見なかったことにする
#pragma warning(disable:26819)
#include  <json.hpp>
#pragma warning(pop)
#include "GlobalVariables/GlobalVariables.h"

#include "IBullet.h"
#include "Character/Player/PlayerBullet/PlayerBullet.h"
#include "Character/Player/PlayerRazer/PlayerRazer.h"
#include "Sprite.h"
#include "PlaneProjectionShadow.h"

#include "Material.h"
#include "DirectionLight.h"
#include "Particle/Particle.h"


class LockOn;
// ふるまい
enum class Behavior {
	kRoot, // 通常状態
	kAttack, // 攻撃中
	kDash, // ダッシュ中
	kJump,
	kTitlePlayer
};
class Player : public Collider
{
public:
	void Init(const Vector3& translate, const std::string filename);
	void Update();
	void Draw(Camera* camera);
	/// <summary>
	/// UI描画
	/// </summary>	
	void DrawUI();


	/// <summary>
	/// Title
	/// </summary>
	void TitleInit();
	void TitleUpdate();

	/// <summary>
	/// 
	/// </summary>
	void ClearInit();
	void ClearUpdate();

	/// <summary>
	/// GameOver
	/// </summary>
	void GameOverInit();
	void GameOverUpdate();
	void GameOverDraw(Camera* camera);

	/// <summary>
	/// Demo
	/// </summary>
	void DemoUpdate();
public:
	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用の媒介変数
		uint32_t dashparam_ = 0;
	};

public:
	void Move();

	void Aim();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	// 浮遊ギミック初期化
	void InitFloatingGimmmick();
	void UpdateFloatingGimmmick();


	/*---敵と衝突した際の処置用関数---*/
	//　衝突した後の衝突クールタイム
	void HitEnemyCoolTime();

	//　敵スライムと衝突した時の処理
	void HitEnemySlime();


	void CreateReticle();

	/*---Behavior用関数---*/
	// 攻撃行動
	void BehaviorRootUpdate();
	void BehaviorRootAttackUpdate();
	// 通常行動
	void BehaviorRootInit();
	void BehaviorRootAttackInit();
	// ダッシュ行動
	void BehaviorRootDashInit();
	void BehaviorRootDashUpdate();
	// ジャンプ行動
	void BehaviorRootJumpInit();
	void BehaviorRootJumpUpdate();
	// title jump
	void BehaviorRootTitleJumpUpdate();



	// 調整項目の適用
	void ApplyGlobalVariables();
public: // AjustParm
	void AdustParm();
	void ColorAdust();

public:
	const std::list<PlayerBullet*>& Getbullet() const { return bullets_; }
	const WorldTransform* GetWorldTransform() const { return &worldTransform_; }
	Behavior GetBehaviorMode() { return behavior_; }
	Vector3 GetReticleWorldPosition();
	// Hpを取得
	float GetHP() { return hp_; }
	uint32_t GetBulletMode() { return bulletMode_; }
	bool IsEnemyHit() { return isEnemyHit_; }
public: // Setter
	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetHP(float hp) { hp_ = hp; }
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }
	void SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; }

	void SetSpotLight(const SpotLight& spotLight) { spotLight_ = spotLight; }
public: // Collider
	void OnCollision(uint32_t attri) override;
	Vector3 GetWorldPosition() const override;

private: // 貸出
	LockOn* lockOn_ = nullptr;
	Camera* camera_ = nullptr;

private:

	std::unique_ptr<PlaneProjectionShadow> shadowObject_;
	std::unique_ptr<PlaneProjectionShadow> reticleShadowObject_;
	std::unique_ptr<Object3d> farReticleObj_;

public: // もともとのゲームで使用変数
	std::unique_ptr<Sprite> hpUI_ = nullptr;
	std::unique_ptr<Sprite> hpUIBlue_ = nullptr;
	std::unique_ptr<Sprite> bulletModeUI = nullptr;


	std::list<PlayerBullet*> bullets_;
	std::list<PlayerRazer*> razers_;
	Vector4 color_;
	//bulletMode
	uint32_t bulletMode_ = NormalBullet;

	// BulletModeUI
	uint32_t playerReticleTex_ = 0;
	uint32_t playerHpUITex_ = 0;
	uint32_t normalBulletUITex_ = 0;
	uint32_t hommingBulletUITex_ = 0;
	uint32_t razerBulletUITex_ = 0;
	/// <summary>
	/// //////////////////////////////////////////////////////////////////////
	/// </summary>
	float hp_ = 1;

private: // Camera
	Vector3 cameraToPlayerDistance_{ 0.0f, 7.0f, -30.0f };

	Vector3 cameraDirection_{ 0.0f,0.0f,1.0f };

private: // Reticle

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticleNear_{};
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticleFar_{};

	// floatY 
	float reticleY_ = 0;

	Vector4 reticleColor_;


private://Bullet
	float bulletSize_ = 0;

private: 
	float slimeBasedAlpha_ = 0.8f;

	float angletime = 0.0f;
	float preAngle_ = 0.0f;
	// 富裕ギミックの媒介変数
	float floatingparam_ = 0.0f;

	// 周期
	const uint16_t period = 120;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * (float)std::numbers::pi * 2.0f / period;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorkDash workDash_{};

	using json = nlohmann::json;

	// 速度
	Vector3 velo_{};
	// ワールド変換データ
	WorldTransform worldTransform_{};
	const float movingRange_ = 195.0f;

	// model skin num
	uint32_t skinTex_ = 0;


	Material material_{};
	DirectionalLight direLight_{};
	SpotLight spotLight_{};

	//　敵と衝突した後の再衝突用タイマー
	bool isEnemyHit_ = false;
	uint32_t coolTimer_ = 0;
	// クールタイム時の透明度調整
	float coolTimeAlpha_ = 0.7f;
	// 透明度の増減値
	float coolTimeAlphaPorM_ = 0.1f;

private:
	bool isShot_ = false;
	uint32_t shotTimer_ = 0;
	const  uint32_t shotInterval_ = 5;
	uint32_t bulletShotSE_ = 0;
	uint32_t jumpSE_ = 0;

private: // GameOver
	std::unique_ptr<Object3d> deadSlimeObj_;
};

