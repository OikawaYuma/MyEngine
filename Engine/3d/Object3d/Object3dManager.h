/**
* @ file ObjectManager.h
* @ brief Objectを管理するクラス Singleton Patternで制作
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Object3d.h"
#include "Camera.h"
#include <map>
#include <string>

class Object3dManager
{

	Object3dManager() = default;
	~Object3dManager() = default;
	const Object3dManager& operator=(const Object3dManager&) = delete;
public:
	static void Init();
	static void Update();
	static void Draw(Camera* camera);
	static void Release();

public: //ソート用関数
	static void ObjectSort();

	static bool CompareAscending(const std::unique_ptr<Object3d>& objectA, const std::unique_ptr<Object3d>& objectB);
	static bool CompareDescending(const std::unique_ptr<Object3d>& objectA, const std::unique_ptr<Object3d>& objectB);

public:
	static void CreateObject(const std::string& objectName);
	static uint32_t StoreObject(const std::string& objectName,WorldTransform* worldTransform,uint32_t skinTex, Vector4* color,uint32_t transparency);

public: 
	static std::shared_ptr<ObjectPram> StoreObject2(const std::string& objectName, uint32_t skinTex, uint32_t transparency);

public: // Getter

	static Object3dManager* GetInstance();

private:
	//std::map<std::string, Object3dData> objectDatas_;
	std::list<std::unique_ptr<Object3d>> objects_;
	static std::string objectName_;
	//ModelData modelData_;
};