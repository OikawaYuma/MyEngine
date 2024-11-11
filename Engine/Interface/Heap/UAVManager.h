/**
* @ file
* @ brief
*/

#pragma once
#include <cstdint>
#include <wrl.h>
#include <d3d12.h>
/*----------------------------------------------------------
   このクラスはシングルトンパターンを元に設計する
--------------------------------------------------------------*/

#pragma once
class UAVManager
{

public:
	void Init();

	static uint32_t Allocate();
private:
	UAVManager() = default;
	~UAVManager() = default;
	const UAVManager& operator=(const UAVManager&) = delete;
public:

	static UAVManager* GetInstance();
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> GetDescriptorHeap() { return descriptorHeap_.Get(); };
	// 最大SRV数（最大テクスチャ枚数）
	static const uint32_t kMaXUAVCount;
	// SRV用のデスクリプタサイズ
	static uint32_t descriptorSize_;
	// SRV用デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// 次に使用するSRVインデックス
	static uint32_t useIndex_;

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// SRV生成（テクスチャ用）
	static void CreateUAVforParm(const TextureData& textureData);
	// SRV生成（Structured Buffer用）
	//static void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	//static void CreateSRVRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	// SRV生成（Depth用）
	//static void CreateSRVDepth(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	//static void CreateSRVRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	void PreDraw();
	static Microsoft::WRL::ComPtr <ID3D12Resource> CreateUAVResource(Microsoft::WRL::ComPtr<ID3D12Device> device, size_t sizeInBytes);

	void SetGraphicsRootDescriptortable(UINT rootParameterIndex, uint32_t srvIndex);
};

