#include "PlaneProjectionShadow.h"
#include "TextureManager.h"
#include "mathFunction.h"
#include "Object3dManager.h"

void PlaneProjectionShadow::Init(WorldTransform* casterWorldTransform, const std::string& filePath)
{
	// 引数をメンバ変数に記録
	casterWorldTransform_ = casterWorldTransform;
	obj_ = std::make_unique<Object3d>();
	obj_->Init();
	obj_->SetModel(filePath+ ".obj");

	Material material;
	material.color = { 0.0f,0.0f,0.0f,1.0f };
	material.enableLighting = false;

	color_ = { 0.0f,0.0f,0.0f,1.0f };
	obj_->SetMaterial(material);
	// 影自体のワールドトランスフォームの初期化
	worldTransform_.Initialize();
	shadowTex_ = TextureManager::GetInstance()->StoreTexture("Resources/uvChecker.png");
	obj_->SetSkinTex(shadowTex_);
	shadowMatrix_ = MakeIdentity4x4();
	// Yの全頂点座標を0にする
	shadowMatrix_.m[1][1] = 0.003f;
	if (casterWorldTransform_) {
		worldTransform_.matWorld_ =
			Multiply(casterWorldTransform_->matWorld_, shadowMatrix_);
	}
	//int i = Object3dManager::GetInstance()->StoreObject(filePath, &worldTransform_, shadowTex_, &material.color);
}

void PlaneProjectionShadow::Update()
{
	if (casterWorldTransform_) {
		worldTransform_.matWorld_ =
			Multiply(casterWorldTransform_->matWorld_, shadowMatrix_);
	}
	//worldTransform_.UpdateMatrix();
	

}

void PlaneProjectionShadow::Draw(Camera* camera)
{
	camera;
	//obj_->Draw(camera);
}
