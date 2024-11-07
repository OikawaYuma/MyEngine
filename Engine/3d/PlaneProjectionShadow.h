/**
* @ file　PlaneProjectionShadow.h
* @ brief 平面でのオブジェクトの影を描画するクラス
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include <string>
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"
class PlaneProjectionShadow
{
public:

	void Init(WorldTransform* casterWorldTransform, const std::string& filePath);
	void Update();
	void Draw(Camera* camera);

private:
	// 影自体のワールド変換データ
	WorldTransform worldTransform_;
	// オブジェクト
	std::unique_ptr<Object3d> obj_;

	// 投影元オブジェクトのワールド変換データ
	WorldTransform* casterWorldTransform_ = nullptr;

	// 平面投影行列
	Matrix4x4 shadowMatrix_{};

	uint32_t shadowTex_ = 0;
};

