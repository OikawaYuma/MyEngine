#include "Object3dManager.h"

void Object3dManager::Init()
{
    objects_.clear();
}

std::list<std::unique_ptr<Object3d>> Object3dManager::objects_;

void Object3dManager::Update()
{
    for (const auto& pair : objects_) {
        pair->Update();
    }
}

void Object3dManager::Draw(Camera* camera)
{
    for (const auto& pair : objects_) {
        pair->Draw(camera);
    }
}

void Object3dManager::ObjectSort()
{
    std::stable_sort(objects_.begin(),objects_.end(),CompareDescending);
}

bool Object3dManager::CompareAscending(const std::unique_ptr<Object3d>& objectA, const std::unique_ptr<Object3d>& objectB)
{
    return objectA->GetTransparency() > objectB->GetTransparency();
}

bool Object3dManager::CompareDescending(const std::unique_ptr<Object3d>& objectA, const std::unique_ptr<Object3d>& objectB)
{
    return objectA->GetTransparency() < objectB->GetTransparency();
}

void Object3dManager::CreateObject(const std::string& objectName)
{
    std::string a = objectName;
}

uint32_t Object3dManager::StoreObject(const std::string& objectName, WorldTransform* worldTransform, uint32_t skinTex, Vector4* color, uint32_t transparency)
{

    for (auto& pair : objects_) {
        if (pair->GetObjectName() == objectName) {
  
            uint32_t objectNum = pair->GetObjectNum();
            // 次のインスタンシング番号にする
           pair->SetWorldTransformInstancing(worldTransform);
           pair->SetMaterialInstancing(color);
           pair->SetObjectNum(objectNum + 1);

            return pair->GetObjectNum();
        };
    }
   




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
    object->SetObjectName(objectName);
    object->SetTransparency(transparency);
    objects_.push_back(std::move(object));
    return objectNum;

   
}

Object3dManager* Object3dManager::GetInstance()
{
    static Object3dManager instance;
    return &instance;

}