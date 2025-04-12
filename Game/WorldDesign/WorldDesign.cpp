#include "WorldDesign.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Model.h"
#include "Object3dManager.h"
#include "Input.h"
WorldDesign::~WorldDesign()
{
	SetObjectPram();
	shadowObject_.reset();
}
void WorldDesign::Init(const Vector3& scale, const Vector3& translate, const std::string filename)
{
	floorTex_ = TextureManager::GetInstance()->StoreTexture("Resources/wood.png");
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	objectPram_ = Object3dManager::GetInstance()->StoreObject2(filename,floorTex_, Transparency::Opaque);
	objectPram_.lock()->worldTransform.scale_ = scale;
	objectPram_.lock()->worldTransform.translation_.x = 5;
	objectPram_.lock()->color = color_;


	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;

	shadowObject_ = std::make_unique<PlaneProjectionShadow>();
	shadowObject_->Init(&objectPram_.lock()->worldTransform, filename);
	shadowObject_->Update();
}

void WorldDesign::Update()
{
	if (objectPram_.lock()) {
		if (Input::GetInstance()->PushKey(DIK_L)) {
			objectPram_.lock()->worldTransform.translation_.x++;
		};
		objectPram_.lock()->worldTransform.UpdateMatrix();
	}
	shadowObject_->Update();
}



void WorldDesign::SetObjectPram()
{
	if (auto locked = objectPram_.lock()) {  // weak_ptrからshared_ptrに変換
		locked->isAlive = false;
	}
}

