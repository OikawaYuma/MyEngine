#include "PSOComputeShader.h"

PSOComputeShader* PSOComputeShader::GatInstance()
{
	static PSOComputeShader instance;
	return &instance;
}

void PSOComputeShader::CreatePipelineStateObject()
{
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	CreateDescriptorRange();

	CreateRootParameter();

	CreateRootSignature();
	
	// Shaderをコンパイルする
	computeShaderBlob = CompileShader(L"Resources/shader/Skinning.CS.hlsl",
		L"vs_6_0", sDirectXCommon->GetDxcUtils(), sDirectXCommon->GetDxcCompiler(), sDirectXCommon->GetIncludeHandler());
	assert(property.vertexShaderBlob != nullptr);

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.CS = {
		.pShaderBytecode = computeShaderBlob->GetBufferPointer(),
		.BytecodeLength = computeShaderBlob->GetBufferSize()
	};

	computePipelineStateDesc.pRootSignature = computeRootSignature.Get();

	HRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateComputePipelineState(
	&computePipelineStateDesc, IID_PPV_ARGS(&computePipelineState));
}

void PSOComputeShader::CreateRootParameter()
{
	// RootParmeter作成。複数でっていできるので配列。
	rootParamerters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParamerters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; 
	rootParamerters[0].Descriptor.ShaderRegister = 0; //レジスタ番号0とバインド

	// RootParmeter作成。複数でっていできるので配列。
	rootParamerters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParamerters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParamerters[1].Descriptor.ShaderRegister = 1; //レジスタ番号1とバインド

	// RootParmeter作成。複数でっていできるので配列。
	rootParamerters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParamerters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParamerters[2].Descriptor.ShaderRegister = 2; //レジスタ番号2とバインド

	// RootParmeter作成。複数でっていできるので配列。
	rootParamerters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParamerters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParamerters[3].Descriptor.ShaderRegister = 3; //レジスタ番号3とバインド

	rootParamerters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParamerters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParamerters[2].DescriptorTable.pDescriptorRanges = &descriptorRange_[0]; // Tableの中身の配列を指定
	rootParamerters[2].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	
}

void PSOComputeShader::CommandRootParameter()
{
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	sDirectXCommon->GetCommandList()->SetComputeRootSignature(computeRootSignature.Get());
	sDirectXCommon->GetCommandList()->SetPipelineState(computePipelineState.Get());

}

void PSOComputeShader::CreateDescriptorRange()
{
	

	descriptorRange_[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange_[0].NumDescriptors = 1; // 数は2つ
	descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
}
