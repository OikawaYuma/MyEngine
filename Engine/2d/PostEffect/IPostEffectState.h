/**
* @ file IPostEffectState.h
* @ brief StatePatternでポストエフェクトの掛けるエフェクトを変更を行うためのインターフェース
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <vector>
#include <d3d12.h>
#include <PSOProperty.h>

enum PostEffectMode {
	kFullScreen,
	kGrayscale,
	kVignetting,
	kGaussianFilter,
	kLuminanceOutline,
	kDepthOutline,
	kRadialBlur,
	kDissolve,
	kRandom,
	kBloom,
	kDissolveOutline,
	kNumPostEffect

};
class PostProcess;
// シーン内での処理を行う基底クラス
class IPostEffectState {
protected:
public:

	/// <summary>
	/// 描画に関する設定をまとめる関数
	/// </summary>
	virtual PSOProperty CreatePipelineStateObject() = 0;

	virtual void CommandRootParameter(PostProcess* postProcess) = 0;
	virtual void Init() = 0;

	/// <summary>
	/// Shaderの配列設定
	/// </summary>
	/// <returns></returns>
	virtual std::vector<D3D12_DESCRIPTOR_RANGE> CreateDescriptorRange() = 0;

	/// <summary>
	/// Shaderに渡す値を決める
	/// </summary>
	/// <returns></returns>
	virtual std::vector<D3D12_ROOT_PARAMETER> CreateRootParamerter(std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRange) = 0;

	/// <summary>
	/// PSにの書き込み方の設定
	/// </summary>
	/// <returns></returns>
	virtual std::vector<D3D12_STATIC_SAMPLER_DESC> CreateSampler() = 0;

	/// <summary>
	/// ShaderとResourceを関係のつけ方を決める関数
	/// </summary>
	virtual D3D12_ROOT_SIGNATURE_DESC CreateRootSignature(PSOProperty& pso, std::vector<D3D12_ROOT_PARAMETER>& rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers) = 0;

	/// <summary>
	/// VSへ渡す頂点データの指定を行う関数
	/// </summary>
	virtual D3D12_INPUT_LAYOUT_DESC SetInputLayout() = 0;

	/// <summary>
	/// PSからの出力をどう書き込むかの設定を行う関数
	/// </summary>
	virtual D3D12_BLEND_DESC SetBlendState() = 0;

	/// <summary>
	/// ラスタライザーに関する設定の関数
	/// </summary>
	virtual D3D12_RASTERIZER_DESC SetRasterrizerState() = 0;

	/// <summary>
	/// DepthBufferの生成
	/// </summary>
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepth() = 0;

	
	// 仮想デストラクタを用意しないと警告される
	virtual ~IPostEffectState();

	PSOProperty GetProperty() { return property_; }
	void SetPropery(PSOProperty property) { property_ = property; };

protected:
	PSOProperty property_;



};