/**
* @ file PSOPostEffect.h
* @ brief ポストエフェクト画像の描画に関する設定　Singleton Patternで作成
* @ author 及川　優麿　（オイカワ　ユウマ）
*/
#pragma once
#include <d3d12.h>
#include "DirectXCommon.h"
#include "PSOProperty.h"
#include "IPostEffectState.h"
#include "Vector3.h"
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>


class PSOPostEffect : public PSOProperty
{
public:
	static PSOPostEffect* GatInstance();

	PSOPostEffect() = default;
	~PSOPostEffect() = default;
	const PSOPostEffect& operator=(const PSOPostEffect&) = delete;

	

	void Init();

	void EffectChangeCheck(uint32_t effectNo);
	void SetCommand(PostProcess* postProcess,uint32_t effectNo);

	

private:
	HRESULT hr_;
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	
	
	// 描画先のRTVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	
	// エフェクトを保持するメンバ変数
	std::unique_ptr<IPostEffectState> effectArr_[PostEffectMode::kNumPostEffect];
	
	// どのステージを呼び出すかを管理する変数
	int currentEffectNo_ = 0;
	int prevEffectNo_ = 0;

};


