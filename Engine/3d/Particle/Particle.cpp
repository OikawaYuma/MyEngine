#include "Particle.h"
#include "Sprite.h"
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "mathFunction.h"
#include "Mesh.h"
#include "SRVManager.h"
#include <numbers>
#include <ModelManager.h>
#include <Object3d.h>



Particle::Particle() {}
Particle::~Particle()
{
}
;
void Particle::Init() {
	sWinAPI = WinAPI::GetInstance();
	sDirectXCommon = DirectXCommon::GetInstance();

	// 頂点リソースを作る
	vertexResource_ = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(VertexData) * model_->GetModelData().vertices.size());

	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * model_->GetModelData().vertices.size()); // 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, model_->GetModelData().vertices.data(), sizeof(VertexData) * model_->GetModelData().vertices.size());

	

	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(Material));

	/*materialBufferView = CreateBufferView();;*/
	// 頂点リソースにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 色のデータを変数から読み込み
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData->uvTransform = MakeIdentity4x4();
	materialData->enableLighting = false;

	// Transform変数の初期化

	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixResouceSprite = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(ParticleForGPU) * kNumMaxInstance);
	// 書き込むためのアドレスを取得
	transformationMatrixResouceSprite->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	// 単位行列を書き込んでおく

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].World = MakeIdentity4x4();
		instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	indexResource_ = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice().Get(), sizeof(uint32_t) * model_->GetModelData().indices.size());
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * static_cast<uint32_t>(model_->GetModelData().indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexDataSprite = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));

	std::memcpy(indexDataSprite, model_->GetModelData().indices.data(), sizeof(uint32_t) * model_->GetModelData().indices.size());


	transformUv = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	SRVIndex_ = SRVManager::Allocate();
	SRVManager::CreateSRVforStructuredBuffer(SRVIndex_, transformationMatrixResouceSprite.Get(), kNumMaxInstance, sizeof(ParticleForGPU));
	instancingSrvHandleCPU = SRVManager::GetCPUDescriptorHandle(SRVIndex_);
	instancingSrvHandleGPU = SRVManager::GetGPUDescriptorHandle(SRVIndex_);

	directionalLightData = nullptr;
	directionalLightResource = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;


	//emitter_.transform = { emitter.transform.scale,{0.0f,0.0f,0.0f},{0.0f,0.0f,20.0f} };
	emitter_.count = 6;
	emitter_.frequency = 0.02f;// 0.5秒ごとに発生
	emitter_.frequencyTime = 0.0f;// 発生頻度用の時刻、0で初期化

	numInstance_ = 0;
}
void Particle::Update(const RandRangePro& randRange, bool scaleAddFlag)
{
	randRange_ = randRange;

	emitter_.transform.translate = worldTransform_.translation_;

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData[index].WVP = worldTransform_.matWorld_;
		instancingData[index].World = worldTransform_.matWorld_;
		instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	randRange_ =
	{
		{0.3f,0.7f},
		{0.2f,0.5f},
		{-0.5f,0.3f}
	};
	//materialData->color = {1.0f,1.0f,1.0f,1.0f};
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, camera_->GetCameraMatrix());
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());


	emitter_.frequencyTime += kDeltaTime;// 時刻を進める
	if (emitter_.frequency <= emitter_.frequencyTime) {// 頻度より大きいなら発生
		particles_.splice(particles_.end(), Emission(emitter_, randomEngine, randRange));
		emitter_.frequencyTime -= emitter_.frequency;// 余計に過ぎた時間も加味して頻度計算する

	}

	numInstance_ = 0;
	// Sprite用のWorldViewProjectMatrixを作る
	for (std::list<ParticlePro>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		(*particleIterator).transform.translate.x += (*particleIterator).velocity.x * kDeltaTime;
		(*particleIterator).transform.translate.y += (*particleIterator).velocity.y * kDeltaTime;
		(*particleIterator).transform.translate.z += (*particleIterator).velocity.z * kDeltaTime;
		if (scaleAddFlag) {
			(*particleIterator).transform.scale = Add((*particleIterator).transform.scale, { 0.1f ,0.1f,0.1f });
		}
		(*particleIterator).currentTime += kDeltaTime;
		// (*particleIterator).color = { 1.0f,1.0f,1.0f,1.0f };
		float alpha = 0.5f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
		//transforms_[index].rotate.x += 0.1f;
		Matrix4x4 worldMatrix = Multiply(MakeScaleMatrix((*particleIterator).transform.scale), Multiply(billboardMatrix, MakeTranslateMatrix((*particleIterator).transform.translate)));
		//Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera_->GetViewMatrix(), camera_->GetProjectionMatrix()));
		if (numInstance_ < kNumMaxInstance) {
			instancingData[numInstance_].WVP = worldViewProjectionMatrix;
			instancingData[numInstance_].World = worldMatrix;
			instancingData[numInstance_].color = (*particleIterator).color;
			instancingData[numInstance_].color.w = alpha;
		}
		++numInstance_; // 生きているparticluの数を1使うんとする
		++particleIterator;

	}

	model_->Update();
};

void Particle::Draw() {
	pso_ = PSOParticle::GatInstance();


	
	sDirectXCommon->GetCommandList()->SetGraphicsRootSignature(pso_->GetProperty().rootSignature.Get());
	sDirectXCommon->GetCommandList()->SetPipelineState(pso_->GetProperty().graphicsPipelineState.Get());    //PSOを設定



	sDirectXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	sDirectXCommon->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sDirectXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアルCBufferの場所を設定
	sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// TransformationmatrixCBufferの場所を設定
	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetGPUDescriptorHandle(tex_));
	sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	// 描画（DrawCall/ドローコール）
	//sDirectXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	sDirectXCommon->GetCommandList()->DrawIndexedInstanced(static_cast<uint32_t>(model_->GetModelData().indices.size()), numInstance_, 0, 0, 0);
}




Particle::ParticlePro Particle::MakeNewParticle(std::mt19937& randomEngine, const Vector3& scale, const Vector3& translate, const RandRangePro& randRange)
{
	RandRangePro ran = randRange;
	std::uniform_real_distribution<float> distribution(-0.8f, 0.8f);
	std::uniform_real_distribution<float> distriposX(ran.rangeX.x, ran.rangeX.y);// distriposX(-0.7f, -0.3
	std::uniform_real_distribution<float> distriposY(ran.rangeY.x, ran.rangeY.y);// distriposY(0.2f, 0.5f)
	std::uniform_real_distribution<float> distriposZ(ran.rangeZ.x, ran.rangeZ.y);// distriposZ(-0.5f, 0.3f
	//std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(0.0f, 1.5f);

	Particle::ParticlePro particle;
	particle.transform.scale = scale;
	particle.transform.rotate = { 0.0f,0.0f,0.0f };

	// 位置と速度を[-1,1]でランダムに初期化
	Vector3 randomTranslate = { distriposX(randomEngine), distriposY(randomEngine), distriposZ(randomEngine) };
	particle.transform.translate = Add(translate, randomTranslate);
	particle.velocity = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.color = { 1.0f,1.0f ,1.0f,0.7f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}

std::list<Particle::ParticlePro> Particle::Emission(const Emitter& emitter, std::mt19937& randEngine, const RandRangePro& randRange)
{
	std::list<Particle::ParticlePro> particles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle(randEngine, emitter.transform.scale, emitter.transform.translate, randRange));

	}
	return particles;
}


void Particle::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
};
