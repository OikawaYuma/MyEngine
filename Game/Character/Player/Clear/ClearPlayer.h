/**
* @ file ClearPlayer.h
* @ brief ClearSceneでの自機Objectの設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <string>
#include <memory>
#include "Vector3.h"
#include "Vector4.h"
#include "WorldTransform.h"
#include "PlaneProjectionShadow.h"

class ClearPlayer
{
public:
	void Init();
	void Update();

public: // 行動
	// ジャンプ
	void Jump();

private:
	// ワールド座標
	WorldTransform worldTransform_{};
	// RGBA情報
	Vector4 color_{};
	// テクスチャ番号
	uint32_t skinTex_ = 0;
	// 影
	std::unique_ptr<PlaneProjectionShadow> shadowObject_ = nullptr;
	// 速度
	Vector3 velo_{ 0.0f,0.0f,0.0f };
	// ジャンプ用のフラグ
	bool isJump_ = false;
};

