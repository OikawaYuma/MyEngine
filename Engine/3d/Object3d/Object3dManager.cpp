#include "Object3dManager.h"

void Object3dManager::Init()
{
    objects_.clear();
}

std::map<std::string, std::unique_ptr<Object3d>> Object3dManager::objects_;

void Object3dManager::Update()
{
    for (const auto& pair : objects_) {
        pair.second->Update();
    }
}

void Object3dManager::Draw(Camera* camera)
{
    for (const auto& pair : objects_) {
        pair.second->Draw(camera);
    }
}

void Object3dManager::CreateObject(const std::string& objectName)
{
}

uint32_t Object3dManager::StoreObject(const std::string& objectName, WorldTransform* worldTransform, uint32_t skinTex, Vector4* color)
{
    if (objects_.contains(objectName)) {
        Object3d* object = objects_[objectName].get();
        uint32_t objectNum = object->GetObjectNum();
        // 次のインスタンシング番号にする
        object->SetWorldTransformInstancing(worldTransform);
        object->SetMaterialInstancing(color);
        object->SetObjectNum(objectNum + 1);
       
        return object->GetObjectNum();
    };
    std::unique_ptr<Object3d> object = std::make_unique<Object3d>();
    object->Init();
    object->SetModel(objectName + ".obj");
    object->SetMaterialInstancing(color);
    uint32_t objectNum = object->GetObjectNum();
    // 次のインスタンシング番号にする
    object->SetWorldTransformInstancing(worldTransform);
    // 次のインスタンシング番号にする
    object->SetObjectNum(objectNum + 1);
    object->SetSkinTex(skinTex);
    objects_[objectName] = std::move(object);
    return objectNum;

   
}

Object3dManager* Object3dManager::GetInstance()
{
    static Object3dManager instance;
    return &instance;

}