#pragma once
#include "Character/Enemy/Enemy.h"
#include <list>
#include <memory>
class EnemyApear
{
public:
	void Init();
	void Update(std::list<std::unique_ptr<Enemy>>& enemys);
	
public:
	void SetPlayer(Player* player) { player_ = player; };
private:
	Player* player_ = nullptr;
	
	int occurrenceNum_ = 4;

	int appearranceTimer_ = 0;

	const int appearranceTime = 120;

	bool sortFlag_ = false;

};

