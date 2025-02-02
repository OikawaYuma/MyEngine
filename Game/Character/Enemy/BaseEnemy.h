/**
* @ file BaseEnemy.h
* @ brief Enemyに共通の項目をまとめた基底クラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <string>
#include "Collider.h"
class Player;
class BaseEnemy : public Collider {
public:// 抽象クラスなので、純粋仮想仮想関数とする
	virtual void Init(const Vector3& translate, const std::string filename) = 0;
	virtual void Update() = 0;

	// 仮想デストラクタを用意
	virtual ~BaseEnemy();

public: // Getter
	uint32_t GetHP() { return HP_; }


public: // Setter
	void SetHP(uint32_t HP) { HP_ = HP; }
	virtual void SetPlayer(Player* player) = 0;


protected:

	uint32_t HP_ = 3;

};