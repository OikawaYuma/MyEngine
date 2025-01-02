#include "PSOPostEffect.h"
#include "Effect/GrayScale.h"
#include "Effect/Vignette.h"
#include "Effect/LuminanceOutline.h"
#include "Effect/DepthOutline.h"
#include "Effect/Dissolve.h"
#include "Effect/FullScreen.h"
#include "Effect/GaussianBlur.h"
#include "Effect/RadialBlur.h"
#include "Effect/Random.h"
#include "Effect/Bloom.h"
#include "Effect/DissolveOutline.h"
#include <d3dx12.h>



void PSOPostEffect::Init()
{
	effectArr_[PostEffectMode::kFullScreen] = std::make_unique<FullScreen>();
	effectArr_[PostEffectMode::kGrayscale] = std::make_unique<GrayScale>();
	effectArr_[PostEffectMode::kVignetting] = std::make_unique<Vignette>();
	effectArr_[PostEffectMode::kGaussianFilter] = std::make_unique<GaussianBlur>();
	effectArr_[PostEffectMode::kDissolve] = std::make_unique<Dissolve>();
	effectArr_[PostEffectMode::kLuminanceOutline] = std::make_unique<LuminanceOutline>();
	effectArr_[PostEffectMode::kDepthOutline] = std::make_unique<DepthOutline>();
	effectArr_[PostEffectMode::kRadialBlur] = std::make_unique<RadialBlur>();
	effectArr_[PostEffectMode::kRandom] = std::make_unique<Random>();
	effectArr_[PostEffectMode::kBloom] = std::make_unique<Bloom>();
	effectArr_[PostEffectMode::kDissolveOutline] = std::make_unique<DissolveOutline>();
	for (int i = 0; i < PostEffectMode::kNumPostEffect; i++) {
		effectArr_[i]->Init();
	}
}

void PSOPostEffect::EffectChangeCheck(uint32_t effectNo)
{
	//// シーンのチェック
	prevEffectNo_ = currentEffectNo_;
	currentEffectNo_ = effectNo;

	//// シーン変更チェック
	//if (prevEffectNo_ != currentEffectNo_) {
	//	effectArr_[effectNo]->Init();
	//	property = effectArr_[effectNo]->CreatePipelineStateObject();
	//}
}

void PSOPostEffect::SetCommand(PostProcess* postProcess, uint32_t effectNo)
{	
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(effectArr_[effectNo]->GetProperty().rootSignature.Get());
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(effectArr_[effectNo]->GetProperty().graphicsPipelineState.Get());    //PSOを設定

	effectArr_[effectNo]->CommandRootParameter(postProcess);
}




PSOPostEffect* PSOPostEffect::GatInstance() {
	static PSOPostEffect instance;
	return &instance;
}
