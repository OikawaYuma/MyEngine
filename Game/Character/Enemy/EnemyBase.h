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