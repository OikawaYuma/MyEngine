#include "UAVManager.h"
#include "VertexData.h"
#include "DirectXCommon.h"

const uint32_t UAVManager::kMaXUAVCount = 512;

uint32_t UAVManager::useIndex_ = 1;

// SRV用のデスクリプタサイズ
uint32_t UAVManager::descriptorSize_;

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> UAVManager::descriptorHeap_;

void UAVManager::Init()
{
    // デスクリプタヒープの生成
    descriptorHeap_ = DirectXCommon::GetInstance()->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaXUAVCount, true);
    // デスクリプタ1個分のサイズを取得して記録
    descriptorSize_ = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

uint32_t UAVManager::Allocate()
{
    // return する番号をいったん記録しておく
    int index = useIndex_;
    // 次回のために番号を1進める
    useIndex_++;
    // 上で記録した番号をreturn
    return index;
}

UAVManager* UAVManager::GetInstance()
{
    static UAVManager instance;
    return &instance;
}

D3D12_CPU_DESCRIPTOR_HANDLE UAVManager::GetCPUDescriptorHandle(uint32_t index)
{
    D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    handleCPU.ptr += (descriptorSize_ * index);
    return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE UAVManager::GetGPUDescriptorHandle(uint32_t index)
{
    D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
    handleGPU.ptr += (descriptorSize_ * index);
    return handleGPU;
}

void UAVManager::CreateUAVforParm(const TextureData& textureData)
{
    D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
    // metaDataを基にSRVの設定
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement = 0;
    uavDesc.Buffer.NumElements;
    uavDesc.Buffer.CounterOffsetInBytes = 0;
    uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
    uavDesc.Buffer.StructureByteStride = sizeof(VertexData);
    
    // 第二引数は今はnullptrにしておく
    DirectXCommon::GetInstance()->GetDevice()->
        CreateUnorderedAccessView(
            textureData.resource.Get(),
            nullptr,
            &uavDesc,
            GetCPUDescriptorHandle(textureData.srvIndex));

}

Microsoft::WRL::ComPtr<ID3D12Resource> UAVManager::CreateUAVResource(Microsoft::WRL::ComPtr<ID3D12Device> device, size_t sizeInBytes)
{
    //頂点リソース用のヒープの設定
    D3D12_HEAP_PROPERTIES uploadHeapProperties{};
    uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// Uploadheapを使う
    D3D12_RESOURCE_DESC resourceDesc{};
    // バッファリソース。テクスチャの場合はまた別の設定をする
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = sizeInBytes;// リソースのサイズ。今回はVector4を3頂点分
    //バッファの場合はこれらは1にする決まり
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    // バッファに場合はこれにする決まり
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = ;

    Microsoft::WRL::ComPtr <ID3D12Resource> resource = nullptr;
    HRESULT hr;
    hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
        &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&resource));
    assert(SUCCEEDED(hr));
    resource->SetName(L"aaaaaaa");
    return resource;
}


