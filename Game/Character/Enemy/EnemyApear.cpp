#include "EnemyApear.h"
#include "Enemy.h"
#include <random>

void EnemyApear::Init()
{
	appearranceTimer_ = 0;
}

void EnemyApear::Update(std::list<std::unique_ptr<Enemy>>& enemys)
{
	appearranceTimer_++;
	if (appearranceTimer_ >= appearranceTime) {

		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());

		
		for (int i = 0; i < occurrenceNum_; i++) {
			std::uniform_real_distribution<float> distriposX(-50, 50);// distriposX(-0.7f, -0.3
			std::uniform_real_distribution<float> distriposZ(-50, 50);// distriposY(0.2f, 0.5f)
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
			enemy->Init({ distriposX(randomEngine),1,distriposZ(randomEngine) },"enemy");
			enemy->SetPlayer(player_);
			enemys.push_back(std::move(enemy));
		}
		appearranceTimer_ = 0;

	}
}
