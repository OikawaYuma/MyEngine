/**
* @ file Enemy.h
* @ brief 出現する敵Objectの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include "Camera.h"
#include "Character/base/BaseCharacter.h"
#include "BaseEnemy.h"
#include "PlaneProjectionShadow.h"
class Player;
class Enemy: public BaseEnemy
{
public:
	void Init(const Vector3& translate, const std::string filename) ;
	void Update();
	void Draw(Camera* camera) ;
	
	/// <summary>
	/// ゲームオーバー
	/// </summary>
	void GameOverUpdate();

public: //基本関数
	// 移動
	void Move();
	// 復活
	void Respown();

public: // Collider
	void OnCollision(uint32_t attri) override;

public: // Getter
	 Vector3 GetWorldPosition() const override ;
	 bool IsDead() { return isDead_; }

public: // Setter
	 void SetPlayer(Player* player) override { player_ = player; }

private:
	// 平行影
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

private: // 共通項目の変数
	// Enemyで共有する撃破数
	static int enemyDestory_;

public: // 共通項目の関数
	static int GetEnemyDestory() { return enemyDestory_; }
	static void SetEnemydestory(int32_t resttNum) { enemyDestory_ = resttNum; }

};

