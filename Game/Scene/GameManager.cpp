#include "GameManager.h"
#include <Windows.h>
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "SRVManager.h"
#include "PSOModel.h"
#include "PSOAnimationModel.h"
#include "PSOSprite.h"
#include "PSOParticle.h"
#include "PSOSkybox.h"
#include "PSOPostEffect.h"
#include "PostProcess.h"
#include "Input.h"
#include "Mesh.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Audio.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dCommon.h"
#include "Triangle.h"

#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "mathFunction.h"
#include <Object3dManager.h>

// コンストラクタ
GameManager::GameManager() {
	// 各シーンの登録
	sceneMap_[TITLE] = [] () { return  std::make_unique<TitleScene>(); };
	sceneMap_[STAGE] = []() { return std::make_unique<GameScene>(); };
	sceneMap_[CLEAR] = []() { return std::make_unique<ClearScene>(); };
	sceneMap_[GAMEOVER] = []() { return  std::make_unique<GameOverScene>(); };
	sceneMap_[DEMO] = []() { return std::make_unique<DemoScene>(); };
	
}

GameManager::~GameManager() {}

const char kWindowTitle[] = "LE2B_05_オイカワユウマ";

int GameManager::Run() {
	//DirectXCommon::D3DResourceLeakChecker leakCheck;

	WinAPI* sWinAPI = WinAPI::GetInstance();
	sWinAPI->Initialize(L"スライムの成長");
	
	DirectXCommon* sDirctX = DirectXCommon::GetInstance();
	sDirctX->Initialize();

	ImGuiCommon *imGuiCommon = ImGuiCommon::GetInstance();
	imGuiCommon->Initialize();

	Audio* sAudio = Audio::GetInstance();
	sAudio->Initialize();

	Object3dCommon* sObjectCommon = Object3dCommon::GetInstance();
	sObjectCommon->Init();

	ModelManager* sModelManager = ModelManager::GetInstance();
	sModelManager->init();

	TextureManager* sTextureManager = TextureManager::GetInstance();
	sTextureManager->Init();

	PSO* pso = PSO::GatInstance();
	pso->CreatePipelineStateObject();

	PSOAnimationModel* psoAnimationModel = PSOAnimationModel::GatInstance();
	psoAnimationModel->CreatePipelineStateObject();

	PSOSprite* psoSprite = PSOSprite::GatInstance();
	psoSprite->CreatePipelineStateObject();

	PSOParticle* psoParticle = PSOParticle::GatInstance();
	psoParticle->CreatePipelineStateObject();

	PSOSkybox* psoskybox = PSOSkybox::GatInstance();
	psoskybox->CreatePipelineStateObject();


	PSOPostEffect* pSOPostEffect = PSOPostEffect::GatInstance();
	pSOPostEffect->Init();
	

	// シーンのチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = IScene::GetSceneNo();

	
	//post->Init();
	sceneArr_ = sceneMap_[currentSceneNo_]();
	sceneArr_->Init();
	

	Input* sInput = Input::GetInstance();
	sInput->Initialize();
	// ウィンドウの×ボタンが押されるまでループ
	while (true)  // ゲームループ
	{
		// Windowsのメッセージ処理
		if (sWinAPI->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		}
		// ゲームの処理の開始
		// PostEffectかけない処理
		sDirctX->tempRender();
		// ImGui
		ImGuiCommon::GetInstance()->UICreate();
		//ImGuiの更新
		ImGuiCommon::GetInstance()->Update();
		
		sInput->Update();

		/*ImGui::Begin("kakunin");
		ImGui::Text("%d", IScene::GetSceneNo());
		ImGui::End();*/

		// シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = IScene::GetSceneNo();

		// シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_ = sceneMap_[currentSceneNo_]();
			sceneArr_->Init();
		}
		

		///
		/// ↓更新処理ここから
		///
		sceneArr_->Update(); // シーンごとの更新処理

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		sceneArr_->Draw();
	
		///
		/// ↑描画処理ここまで
		///
		sDirctX->BeginFrame();
		sDirctX->ChangeDepthStatetoRead();
		sceneArr_->PostDraw();
		sDirctX->ChangeDepthStatetoRender();
		// PostEffectかけないスプライトのDraw
		sceneArr_->Draw2d();

		// フレームの終了
		//スワップチェーン
		sDirctX->ViewChange();
		sAudio->GetIXAudio().Reset();
		// ESCキーが押されたらループを抜ける
		if (sceneArr_->GameClose()) {
			sceneArr_->Release();
			break;
		}
	}

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");
	

	/*------------------------------------------------------------

	-------------------------------------------------------------*/

	sModelManager->Finalize();
	sWinAPI->Finalize();
	//Object3dManager::GetInstance()->Release();
	TextureManager::GetInstance()->Release();
	sDirctX->Release();
	return 0;
}