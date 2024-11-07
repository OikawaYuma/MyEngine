#include "PlaneProjectionShadow.h"
#include "TextureManager.h"
#include "mathFunction.h"

void PlaneProjectionShadow::Init(WorldTransform* casterWorldTransform, const std::string& filePath)
{
	// 引数をメンバ変数に記録
	casterWorldTransform_ = casterWorldTransform;
	obj_ = std::make_unique<Object3d>();
	obj_->Init();
	obj_->SetModel(filePath);

	Material material;
	material.color = { 0.0f,0.0f,0.0f,1.0f };
	material.enableLighting = false;
	obj_->SetMaterial(material);
	// 影自体のワールドトランスフォームの初期化
	worldTransform_.Initialize();
	shadowTex_ = TextureManager::GetInstance()->StoreTexture("Resources/uvChecker.png");
	shadowMatrix_ = MakeIdentity4x4();
	// Yの全頂点座標を0にする
	shadowMatrix_.m[1][1] = 0.003f;
}

void PlaneProjectionShadow::Update()
{
	if (casterWorldTransform_) {
		worldTransform_.matWorld_ =
			Multiply(casterWorldTransform_->matWorld_, shadowMatrix_);
	}
	//worldTransform_.UpdateMatrix();
	
	obj_->Update();
	obj_->SetWorldTransform(worldTransform_);
}

void PlaneProjectionShadow::Draw(Camera* camera)
{
	obj_->Draw(shadowTex_,camera);
}
