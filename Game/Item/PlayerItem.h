/**
* @ file
* @ brief
*/

#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Collider.h"

class Player;
class PlayerItem : public Collider
{

public:
	void Init(const Vector3& translate, const std::string filename);
	void Update();
	void Draw(Camera* camera);

	bool IsDead() { return isDead_; }

public:
	// 衝突を検出したらコールバック関数
	void OnCollision(uint32_t attri)override;

public: // Getter

	Vector3 GetWorldPosition() const override;

public: // Setter
	void SetPlayer(Player* player) { player_ = player; }

private:
	Player* player_ = nullptr;
	std::unique_ptr<Object3d> object_;

	bool isDead_ = false;
	uint32_t floorTex_ = 0;
	WorldTransform worldTransform_;


};
