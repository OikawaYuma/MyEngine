#include "Score.h"
#include "TextureManager.h"
void Score::Init(Vector2 pos, Vector2 size, bool isSum,float width)
{
	for (int i = 0; i < 10;i++) {
		gameNumber_[i] = TextureManager::GetInstance()->StoreTexture("Resources/number/number" + std::to_string(i)+".png");
	}

	for (int i = 0; i < 6; i++) {
		scoreSprite_[i] = std::make_unique<Sprite>();
		scoreSprite_[i]->Init(
			{ i * width + pos.x,pos.y }, // gameScene x 32 y 96
			size,
			{ 0.5f , 0.5f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			"Resources/Reticle.png");
	}
	isSum_ = isSum;
}

void Score::Update(const int& time, const int& killCount)
{
	if (isSum_) {
		sumScore_ = (60 - time) * 10 + killCount * 50;
	}
	

	for (int i = 0; i < 6; i++) {
		digitScore_[i] = (sumScore_ / static_cast<int>(pow(10,5- i))) % 10;
		scoreSprite_[i]->Update();
	}
}

void Score::Draw()
{
	for (int i = 0; i < 6; i++) {
		scoreSprite_[i]->Draw(gameNumber_[digitScore_[i]],{1.0f,1.0f,1.0f,1.0f});
	}
}
