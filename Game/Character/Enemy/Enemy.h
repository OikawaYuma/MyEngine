/**
* @ file
* @ brief
*/

#pragma once
#include <memory>
#include "Camera.h"
#include "Character/base/BaseCharacter.h"
#include "EnemyBase.h"
#include "PlaneProjectionShadow.h"
class Player;
class Enemy: public EnemyBase
{
public:
	void Init(const Vector3& translate, const std::string filename) ;
	void Update();
	void Draw(Camera* camera) ;


	/// <summary>
	/// 
	/// </summary>
	void ClearInit();
	void ClearUpdate();
	void ClearDraw(Camera* camera);

public:
	void GameOverUpdate();
public:
	void Move();

	void Respown();
public: // Collider
	void OnCollision(uint32_t attri) override;
	 Vector3 GetWorldPosition() const override ;
	 bool IsDead() { return isDead_; }
	 void SetPlayer(Player* player) { player_ = player; }
private:
	std::unique_ptr<PlaneProjectionShadow> shadowObject_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	Vector4 color_;
	// model skin num
	uint32_t skinTex_ = 0;
	bool isDead_ = false;
	int radius_ = 1;
	Player* player_ = nullptr;
	Vector3 move{ 0,0,-0.1f };
	float glavity_= 0.0f;
	float hp_ = 1.0f;

	DirectionalLight direLight_{};

	// 行動によるサイズ変更
	Vector3 actionSize_{};
	// サイズ変更の大きさ
	Vector3 actionSizeParm_{  };
	int respownTimer_ = 0;
private: // GameOver
	std::unique_ptr<Object3d> deadSlimeObj_;


private: // 共通項目の管理
	static int enemyDestory_;
public:
	static int GetEnemyDestory() { return enemyDestory_; }
	static void SetEnemydestory(int32_t resttNum) { enemyDestory_ = resttNum; }

};

