/**
* @ file PlaneProjectionShadow.h
* @ brief 表示するObjectの平行面用の影を生成するクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
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
	~PlaneProjectionShadow();
	void Init(WorldTransform* casterWorldTransform, const std::string& filePath);
	void Update();
	void Draw(Camera* camera);
public:

	void SetColor(Vector4 color) { color_ = color; }

	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	Vector4* GetColor() { return &color_; }
	void DeleteShadow();
private:
	// 影自体のワールド変換データ
	WorldTransform worldTransform_;

	std::weak_ptr<ObjectPram> objectPram_{};
	// 影の色
	Vector4 color_;

	// 投影元オブジェクトのワールド変換データ
	WorldTransform* casterWorldTransform_ = nullptr;

	// 平面投影行列
	Matrix4x4 shadowMatrix_{};

	uint32_t shadowTex_ = 0;
};

