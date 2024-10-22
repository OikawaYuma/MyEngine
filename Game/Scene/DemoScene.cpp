#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "IPostEffectState.h"
#include "Loder.h"
#include<stdio.h>
#include<time.h>


void DemoScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.y = 1;
	cameraPos.z = -15;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();
	textureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	textureHandle2 = TextureManager::StoreTexture("Resources/white.png");
	textureHandle3 = TextureManager::StoreTexture("Resources/rostock_laage_airport_4k.dds");
	demoSprite = new Sprite();
	demoSprite->Init({ 0.0f,0.0f }, { 600.0f,600.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = true;
	worldTransform.Initialize();
	worldTransform.translation_.x = 0.0f;
	worldTransform.scale_ = { 1,1,1 };
	worldTransform2.Initialize();
	worldTransform2.translation_.x = 5.0f;
	worldTransform2.scale_ = { 2.0f,2.0f,2.0f };
	worldTransform3.Initialize();
	worldTransform3.translation_.x = -5.0f;
	worldTransform3.scale_ = { 2.0f,1.0f,2.0f };
	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	worldTransform3.UpdateMatrix();
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();
	
	skybox_ = new Skybox();
	skybox_->Init(material);

	ModelManager::GetInstance()->LoadAnimationModel("Resources/human", "sneakWalk.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/player", "player.obj");
	ModelManager::GetInstance()->LoadModel("Resources/box", "box.obj");
	;
	object3d = new Object3d();
	object3d->Init();
	object3d2 = new Object3d();
	object3d2->Init();
	object3d3 = new Object3d();
	object3d3->Init();

	object3d3->SetMapTexture(textureHandle3);
	object3d2->SetMapTexture(textureHandle3);
	//object3d->SetSkybox(skybox_);
	object3d->SetAnimationModel("sneakWalk.gltf");
	object3d2->SetModel("player.obj");
	object3d3->SetModel("box.obj");
    particle = new Particle();
    particle2 = new Particle();

	demoRandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{0.0f,2.0f}
	};
	
	demoEmitter_.count = 6;
	demoEmitter_.frequency = 0.02f;
	demoEmitter_.frequencyTime = 0.0f;
	demoEmitter_.transform.scale = { 0.5f,0.5f,0.5f };
	particle->Initialize(demoEmitter_);
	particle2->Initialize(demoEmitter_);

	IPostEffectState::SetEffectNo(PostEffectMode::kFullScreen);
}

void DemoScene::Update()
{
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState()) {
	}

	short leftStickX = joyState.Gamepad.sThumbLX;



	ImGui::Begin("OBB,BALL");

	ImGui::SliderFloat3("BALLro", &worldTransform2.rotation_.x, -5.0f, 10.0f);
	ImGui::SliderFloat3("BALLtra", &worldTransform2.translation_.x, -5.0f, 10.0f);

	ImGui::SliderFloat3("boxsca", &worldTransform3.rotation_.x, -5.0f, 10.0f);
	ImGui::SliderFloat3("boxtran", &worldTransform3.translation_.x, -5.0f, 10.0f);

	ImGui::End();



	Vector3 camerattt = camera->GetRotate();
	if (Input::GetInstance()->PushKey(DIK_A)) {
		camerattt.x -= 0.08f;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		camerattt.x += 0.08f;
	}
	if (Input::GetInstance()->PushKey(DIK_W)) {
		camerattt.y += 0.08f;
	}if (Input::GetInstance()->PushKey(DIK_S)) {
		camerattt.y -= 0.08f;
	}
	camera->SetRotate(camerattt);
	sceneTime++;
	////カメラの更新
	camera->Update();
	demoSprite->Update();
	
	
	PostEffectChange();

	
	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		rotateSize_ = 0.0f;
	}
	if (Input::GetInstance()->TriggerKey(DIK_D)) {
		rotateSize_ = 0.05f;
	}
	
	object3d->SetWorldTransform(worldTransform);
	object3d2->SetWorldTransform(worldTransform2);
	object3d3->SetWorldTransform(worldTransform3);
	
	object3d->Update();
	object3d2->Update();
	object3d3->Update();

	/*if (IsCollisionAABB(worldTransform3.translation_, worldTransform3.scale_, worldTransform2.translation_, worldTransform2.scale_.x)) {
		boxTexFlag = false;
	}
	else {
		boxTexFlag = true;
	}*/

	

}
void DemoScene::Draw()
{
	//for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
	//	(*itr)->Draw(textureHandle, camera);
	//}
	//demoSprite->Draw(textureHandle,{1.0f,1.0f,1.0f,1.0f});
	
	object3d->Draw(textureHandle,camera);
	object3d2->Draw(textureHandle, camera);
	if (boxTexFlag) {
		object3d3->Draw(textureHandle2, camera);
	}
	else {
		object3d3->Draw(textureHandle, camera);
	}
	//particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z +5}, textureHandle, camera, demoRandPro, false);
	//particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z +5}, textureHandle2, camera, demoRandPro, false);
}

void DemoScene::PostDraw()
{
	postProcess_->Draw();
}

void DemoScene::Draw2d()
{
}

void DemoScene::Release() {
}

// ゲームを終了
int DemoScene::GameClose()
{
	return false;
}

void DemoScene::PostEffectChange()
{
	ImGui::Begin("PostEffect");
	Vector2 viggnetDarkness = postProcess_->GetDarkness();
	float gauss = postProcess_->GetDeviation();
	float threa = postProcess_->GetThreshold();
	time_t currentTime = time(nullptr);
	BloomInfo bloomInfo = postProcess_->GetBloominfo();
	srand(unsigned int(currentTime));
	int eye = rand() % 70 + 1;
	Vector2 randaa = { float(eye),float(rand() % 90 + 2) };
	hsv = {
		postProcess_->GetHSVInfo().hue,
		postProcess_->GetHSVInfo().saturation,
		postProcess_->GetHSVInfo().value,
	};
	if (ImGui::TreeNode("Base(now hsv)")) {
		if (ImGui::Button("Base On ")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kFullScreen);
		}


		ImGui::SliderFloat("hue", &hsv.x, -1.0f, 1.0f);
		ImGui::SliderFloat("saturation", &hsv.y, -1.0f, 1.0f);
		ImGui::SliderFloat("value", &hsv.z, -1.0f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GrayScale")) {
		if (ImGui::Button("GrayScale On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kGrayscale);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Viggnet")) {
		if (ImGui::Button("Viggnet On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kVignetting);
		}
		ImGui::SliderFloat("darkness 1", &viggnetDarkness.x, 0.0f, 16.0f);
		ImGui::SliderFloat("darkness 2", &viggnetDarkness.y, 0.0f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GaussianFilter")) {
		if (ImGui::Button("Gaussian On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kGaussianFilter);
		}
		ImGui::SliderFloat("Devaition", &gauss, 0.01f, 10.0f);
		ImGui::TreePop();
	}




	if (ImGui::TreeNode("DepthOutline")) {
		if (ImGui::Button("DepthOutline On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kOutline);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Radial Blur")) {
		if (ImGui::Button("Radial Blur On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kRadialBlur);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Dissolve")) {
		if (ImGui::Button("Dissolve On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kDissolve);
		}
		ImGui::SliderFloat("Devaition", &threa, 0.00f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Random")) {
		if (ImGui::Button("Random On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kRandom);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Bloom")) {
		if (ImGui::Button("Bloom On")) {
			IPostEffectState::SetEffectNo(PostEffectMode::kBloom);
		}

		ImGui::SliderFloat("luminance", &bloomInfo.luminance, 0.2f, 0.7f);
		ImGui::SliderFloat("Devaition", &bloomInfo.deviation, 0.0f, 10.0f);
		ImGui::SliderFloat("brightness", &bloomInfo.brightness, 0.0f, 10.0f);
		ImGui::SliderInt("kaernel", &bloomInfo.kernelSize, 1, 10);
		ImGui::TreePop();
	}
	ImGui::End();
	postProcess_->SetDarkness(viggnetDarkness);
	postProcess_->SetDeviation(gauss);
	postProcess_->SetThreshold(threa);
	postProcess_->Setrandom(randaa);
	postProcess_->SetBloomInfo(bloomInfo);
	postProcess_->SetHSVInfo({ hsv.x,hsv.y,hsv.z });
}
