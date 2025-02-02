/**
* @ file EnemyApear.h
* @ brief 敵の出現に関する設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Character/Enemy/Enemy.h"
#include <list>
#include <memory>
class EnemyApear
{
public:
	void Init();
	void Update(std::list<std::unique_ptr<BaseEnemy>>& enemys);
	
public: // Setter
	void SetPlayer(Player* player) { player_ = player; };

private:// 貸出
	Player* player_ = nullptr;
private:
	// 同時出現数
	int occurrenceNum_ = 4;
	// 経過時間
	int appearranceTimer_ = 0;
	// 出現時間
	const int appearranceTime = 120;
	// 初出現時にDraw順をソートするための変数
	bool sortFlag_ = false;

};

