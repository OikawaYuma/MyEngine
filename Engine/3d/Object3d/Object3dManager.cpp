#include "Object3dManager.h"

void Object3dManager::Init()
{
    Object3dManager::GetInstance()->objects_.clear();
}


void Object3dManager::Update()
{
    for (const auto& pair : Object3dManager::GetInstance()->objects_) {
        pair->Update();
    }
}

void Object3dManager::Draw(Camera* camera)
{
    for (const auto& pair : Object3dManager::GetInstance()->objects_) {
        pair->Draw(camera);
    }
}

void Object3dManager::Release()
{
    for (auto& pair : Object3dManager::GetInstance()->objects_) {
        pair->Release(); // ComPtr は自動的に解放するので、これで十分
        // srvIndex や他のリソースも適切に解放する必要があれば、ここで行う
    }
}

void Object3dManager::ObjectSort()
{
    std::stable_sort(Object3dManager::GetInstance()->objects_.begin(), Object3dManager::GetInstance()->objects_.end(),CompareDescending);
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

    for (auto& pair : Object3dManager::GetInstance()->objects_) {
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
    Object3dManager::GetInstance()->objects_.push_back(std::move(object));
    return objectNum;

   
}

std::shared_ptr<ObjectPram> Object3dManager::StoreObject2(const std::string& objectName, uint32_t skinTex, uint32_t transparency)
{
    for (auto& pair : Object3dManager::GetInstance()->objects_) {
        if (pair->GetObjectName() == objectName) {

            std::shared_ptr<ObjectPram> objectPram = std::make_shared<ObjectPram>();
            objectPram->isAlive = true;
            objectPram->worldTransform.Initialize();
            objectPram->color = { 1.0f,1.0f,1.0f,1.0f };
            pair->AddListPram(objectPram);

            return objectPram;
        };
    }
    std::unique_ptr<Object3d> object = std::make_unique<Object3d>();
    object->Init();
    object->SetModel(objectName + ".obj");
    object->SetSkinTex(skinTex);
    object->SetObjectName(objectName);
    object->SetTransparency(transparency);
    
    std::shared_ptr<ObjectPram> objectPram = std::make_shared<ObjectPram>();
    objectPram->isAlive = true;
    objectPram->worldTransform.Initialize();
    objectPram->color = { 1.0f,1.0f,1.0f,1.0f };
    object->AddListPram(objectPram);
    Object3dManager::GetInstance()->objects_.push_back(std::move(object));
    return objectPram;
}

Object3dManager* Object3dManager::GetInstance()
{
    static Object3dManager instance;
    return &instance;

}