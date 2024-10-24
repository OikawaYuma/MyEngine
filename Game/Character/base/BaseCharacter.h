/**
* @ file
* @ brief
*/

#pragma once
#include <list>
#include <memory>
#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Collider.h"
class BaseCharacter 
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw(Camera* camera);
public: //Getter
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	
protected:
	// モデルデータ管理
	std::list<Object3d*> objects_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// model skin num
	uint32_t skinTex_;
 };

