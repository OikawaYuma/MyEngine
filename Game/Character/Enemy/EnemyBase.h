/**
* @ file EnemyBase.h
* @ brief Enemyに共通の項目をまとめた基底クラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Collider.h"

class EnemyBase : public Collider {

public: // Getter
	uint32_t GetHP() { return HP_; }

public: // Setter
	void SetHP(uint32_t HP) { HP_ = HP; }

private:

	uint32_t HP_ = 3;

};