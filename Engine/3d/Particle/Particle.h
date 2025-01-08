/**
* @ file Particle.h
* @ brief Particleを生成するクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "PSOParticle.h"

#include <DirectXTex.h>
#include <string>
#include <string>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>
#include <random>
#include "Model.h"

#include "function.h"
#include <wrl.h>
#include "Transform.h"
#include "VertexData.h"
#include "Vector4.h"
#include"Vector3.h"
#include"Vector2.h"
#include "Matrix4x4.h"
#include "Material.h"
#include "DirectionLight.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
class WinAPI;
class DirectXCommon;
class Camera;
class Mesh;

struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};
struct RandRangePro {
	Vector2 rangeX;
	Vector2 rangeY;
	Vector2 rangeZ;
};

struct BoundP {
	bool isBound;
	float power;
	float gravity;
	
};

struct Emitter {
	Transform transform; //!< エミッタのTransform
	Vector3 velocity_;
	uint32_t count; //!< 発生数
	float frequency; //!< 発生頻度
	float frequencyTime; //!< 頻度用時刻
	RandRangePro randRangeXYZ; //!xyzの発生範囲の設定
	float size;
	BoundP boundPro;
};


class Particle
{
public:
	struct ParticlePro { // プロパティ
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
		bool isfall;
	};


	Particle();
	~Particle();

	void Init();
	void Update(bool scaleAddFlag);
	void Draw();
	void Release();
	ParticlePro MakeNewParticle(std::mt19937& randomEngine);

	std::list<ParticlePro> Emission(std::mt19937& randEngine);

	void CreateParticle();

	void Bound();


public: // Setter
	void SetModel(const std::string& filePath);
	// tex
	void SetTexture(const uint32_t tex) { tex_ = tex; }

	void SetWorldTransform(const WorldTransform& worldTransform) { worldTransform_ = worldTransform; }

	void SetCamera(Camera* camera) { camera_ = camera; }

	void SetEmitter(Emitter emitter) { emitter_ = emitter; }

	void SetMaterial(const Material& material) { *materialData_ = material; }

	void SetDirectionLight(const DirectionalLight& direction) { *directionalLightData = direction; }


private:
	// 借りてくる
	WinAPI* sWinAPI = nullptr;
	DirectXCommon* sDirectXCommon = nullptr;
	PSOParticle* pso_ = nullptr;

	Camera* camera_ = nullptr;

	Model* model_ = nullptr;

	const static uint32_t kNumMaxInstance = 10000; // インスタンス数

	uint32_t tex_ = 0;

	uint32_t numInstance_ = 0; // 描画すべきインスタンス数

	WorldTransform worldTransform_{};
	
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_ = nullptr;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	VertexData* vertexData_ = nullptr;

	// Instancing用のTransformMatrixリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResorce = nullptr;
	ParticleForGPU* instancingData = nullptr;
	// データを書き込む
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResouceSprite;

	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};


	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	// 頂点リソースにデータを書き込む

	Material* materialData_;
	std::list<ParticlePro> particles_;
	//ParticlePro particles_[kNumMaxInstance];
	std::list<Transform>  transforms_;
	

	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	// データを書き込む
	DirectionalLight* directionalLightData;

	Transform transformUv;
	uint32_t SRVIndex_;

	// Δtを定義。とりあえず60fps固定してあるが、
	//実時間を計測して可変fpsで動かせるようにしておくとなお良い
	const float kDeltaTime = 1.0f / 60.0f;
	Emitter emitter_{};
	RandRangePro randRange_;
};
