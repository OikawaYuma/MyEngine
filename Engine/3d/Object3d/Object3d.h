/**
* @ file Object.h
* @ brief Objectの情報を持つクラス
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "PSOModel.h"
#include "ModelData.h"
#include "MaterialData.h"
#include "vertexData.h"
#include "PSOSkybox.h"
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <Material.h>
#include <Transform.h>
#include <Camera.h>
#include <WorldTransform.h>
#include "Model.h"
#include "AnimationModel.h"
#include "Object3dCommon.h"
#include <PSOAnimationModel.h>
#include "Skybox/Skybox.h"
#include "SpotLight.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

// object1個分のデータ
struct Object3dData {
	std::string objectName;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
	uint32_t instancingNum;
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

};

struct Object3dForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
	Vector4 color;
};

struct ObjectPram{
	bool isAlive;
	WorldTransform worldTransform;
	Vector4 color;
};

enum Transparency {
	Opaque, // 不透明
	Transparent, //透明
};
class Object3d
{
public:
	void Init();
	void Update();
	void Draw(Camera* camera);
	void Release();

public: // Setter
	void SetTransparency(uint32_t transparency) { transparency_ = transparency; }
	void SetObjectName(const std::string objectName) { object3dData_.objectName = objectName; }
	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	void SetAnimationModel(const std::string& filePath);
	void SetSkybox(Skybox* skybox) { skybox_ = skybox; }
	void SetWorldTransform(const WorldTransform& worldtransform) { worldTransform_ = worldtransform; };
	void SetWorldTransformInstancing(WorldTransform* worldtransform) { instancingWorld_[objectNum_] = worldtransform; };
	void SetTransform(Transform transform);
	void SetMapTexture(uint32_t maptexture) { mapTexture_ = maptexture; };
	void SetObjectNum(uint32_t objectNum) { objectNum_= objectNum; }
	void SetMaterial(const Material& material) { *materialData_ = material; }
	void SetMaterialInstancing(Vector4* color) { instancingColor_[objectNum_] = color; }
	void SetSkinTex(uint32_t skinTex) { skinTex_ = skinTex; }
	void SetDirectionLight(const DirectionalLight& direction) { *directionalLightData = direction; }
	void SetSpotLight(const SpotLight& spotLight) { *spotLightData_ = spotLight; }
	void SetSpotlightPos(const Vector3& pos) { spotLightData_->position = pos; }
public: // Getter
	uint32_t GetTransparency() { return transparency_; }
	std::string GetObjectName() { return object3dData_.objectName; }
	uint32_t GetObjectNum() { return objectNum_; }
	WorldTransform GetWorldTransform() { return worldTransform_; }
	Material GetMaterial() { return *materialData_; }
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
public: 
	void AddListPram(std::shared_ptr<ObjectPram > objectParm);

private:
	Object3dCommon* objectCommon_ = nullptr;

	Object3dData object3dData_;

	const static uint32_t kNumMaxInstance_ = 10000; // インスタンス数
	uint32_t numInstance_ = 0;
	std::list<std::shared_ptr<ObjectPram>> objectParms_;

	uint32_t objectNum_ = 0;
	WorldTransform* instancingWorld_[kNumMaxInstance_]{nullptr};
	Vector4* instancingColor_[kNumMaxInstance_]{};
	Object3dForGPU* instancingData_ = nullptr;

	uint32_t SRVIndex_ = 0;

	uint32_t skinTex_ = 0;

	// RootSignature作成
	Model* model_ = nullptr;
	AnimationModel* animationModel_ = nullptr;
	Skybox* skybox_ = nullptr;

	Transform transformUv{};

	/*カメラ用*/
	Microsoft::WRL::ComPtr < ID3D12Resource> cameraForGPUResource_;
	CameraForGPU* cameraForGPUData_ = nullptr;
	// データを書き込む
	WorldTransform worldTransform_{};

	// 環境マップ用Texture
	uint32_t mapTexture_ = 1;


	// マテリアルをきれいに
	Material* materialData_ = nullptr;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};

	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	// データを書き込む
	DirectionalLight* directionalLightData = nullptr;

	// スポットライト用
	Microsoft::WRL::ComPtr < ID3D12Resource> spotLightResource_;
	SpotLight* spotLightData_ = nullptr;
	SpotLight spotlight;


	uint32_t transparency_ = 0;
};

