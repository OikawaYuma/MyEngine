#include "PlaneProjectionShadow.h"
#include "TextureManager.h"
#include "mathFunction.h"
#include "Object3dManager.h"

PlaneProjectionShadow::~PlaneProjectionShadow()
{
	objectPram_.lock()->isAlive = false;
}

void PlaneProjectionShadow::Init(WorldTransform* casterWorldTransform, const std::string& filePath)
{
	// 引数をメンバ変数に記録
	casterWorldTransform_ = casterWorldTransform;
	
	shadowTex_ = TextureManager::GetInstance()->StoreTexture("Resources/uvChecker.png");
	objectPram_ = Object3dManager::GetInstance()->StoreObject2(filePath,shadowTex_, Transparency::Opaque);
	objectPram_.lock()->color = { 0.0f,0.0f,0.0f,1.0f };
	// 影自体のワールドトランスフォームの初期化
	worldTransform_.Initialize();
	
	shadowMatrix_ = MakeIdentity4x4();
	// Yの全頂点座標を0にする
	shadowMatrix_.m[1][1] = 0.003f;
	if (casterWorldTransform_) {
		objectPram_.lock()->worldTransform.matWorld_ =
			Multiply(casterWorldTransform_->matWorld_, shadowMatrix_);
	}
	//int i = Object3dManager::GetInstance()->StoreObject(filePath, &worldTransform_, shadowTex_, &material.color);
}

void PlaneProjectionShadow::Update()
{
	if (casterWorldTransform_) {
		objectPram_.lock()->worldTransform.matWorld_ =
			Multiply(casterWorldTransform_->matWorld_, shadowMatrix_);
	}
	//worldTransform_.UpdateMatrix();
	

}

void PlaneProjectionShadow::Draw(Camera* camera)
{
	camera;
	//obj_->Draw(camera);
}

void PlaneProjectionShadow::DeleteShadow()
{
	objectPram_.lock()->isAlive = false;
}
