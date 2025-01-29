/**
* @ file GameObjectCommon.h
* @ brief GameObjectに関する共通部分をまとめたクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "string"
#include "Collision/CollisionManager.h"
class GameObjectCommon : public Collider
{
public:
	// 初期化
	virtual void Init(const std::string& objectName);
	// 更新
	virtual void Update();

private:


};

