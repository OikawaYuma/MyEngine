/**
* @ file GameOverPlayer.h
* @ brief GameOverSceneでの自機Objectの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <string>
#include <memory>
#include "Vector4.h"
#include "WorldTransform.h"

class GameOverPlayer
{
public:
	void Init();

private:
	// ワールド座標
	WorldTransform worldTransform_{};
	// RGBA情報
	Vector4 color_{};
	// テクスチャ番号
	uint32_t skinTex_ = 0;
};
