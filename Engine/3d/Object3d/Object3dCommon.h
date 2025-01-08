/**
* @ file ObjectCommon.h
* @ brief Objectの共通部分をまとめたクラス Singleton Patternで制作
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Camera.h"
class Object3dCommon
{

	Object3dCommon() = default;
	~Object3dCommon() = default;
	const Object3dCommon& operator=(const Object3dCommon&) = delete;
public:
	void Init();
	void Update();
	void Draw();
	
public: // Getter
	Camera* GetDefaultCamera() const { return defaultCamera_; }
	static Object3dCommon* GetInstance();

public: // Setter
	void SetDefaultCamera(Camera* camera) { this->defaultCamera_ = camera; }
	
private:
	Camera* defaultCamera_ = nullptr;
	//ModelData modelData_;
};

