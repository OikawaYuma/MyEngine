#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "IPostEffectState.h"
#include "Loder.h"
#include<stdio.h>
#include<time.h>

#include <Audio.h>
void DemoScene::Init()
{



	
	ModelManager::GetInstance()->LoadModel("Resources/player", "player.obj");
	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	ModelManager::GetInstance()->LoadModel("Resources/ground", "ground.obj");
	particle_ = std::make_unique<Particle>();
	particle_->SetModel("ball.obj");
	
	particle_->Init();
	emitter_.count = 10;
	emitter_.frequency = 5000.0f;
	emitter_.frequencyTime = 0.0f;

	emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter_.transform.scale = { 0.25f,0.25f,0.25f };
	emitter_.transform.translate = { 0.0f,1.0f,25.0f };
	randRangePro_ = {
		{0.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,0.0f}
	};
	emitter_.randRangeXYZ = randRangePro_;
	emitter_.size = worldtransform_.scale_.x;
	emitter_.boundPro.power = 1.0f;
	emitter_.boundPro.gravity = 0.0f;
	emitter_.boundPro.isBound = true;
	particle_->SetEmitter(emitter_);
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({1.1f,3.3f,-8.0f});
	ground_ = std::make_unique<Ground>();
	ground_->Init({0.0f,0.0f,0.0f},{100.0f,1.0f,100.0f},"ground");
	ground_->SetCamera(camera_.get());
	obj_ = std::make_unique<Player>();
	obj_->Init({ 1.0f, 0.5f,10.0f }, "ddd");
	obj_->SetCamera(camera_.get());


	spotLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	spotLight_.position = {2.0f ,1.25f,5.0};
	spotLight_.distance = 7.0f;
	spotLight_.direction =
		Normalize(Vector3{ -1.0f,-1.0f,0.0f });
	spotLight_.intensity = 4.0f;
	spotLight_.dacya = 2.0f;
	spotLight_.cosAngle =
		std::cos(std::numbers::pi_v<float> / 3.0f);

	ModelManager::GetInstance()->LoadModel("Resources/slimeDead", "slimeDead.obj");
	tex_ = TextureManager::GetInstance()->StoreTexture("Resources/player/player.png");
	obj2_ = std::make_unique<Object3d>();
	obj2_->Init();
	obj2_->SetModel("ball.obj");
	obj2_->SetMaterial({.color = {1.0f,1.0f,1.0f,0.8f}});
	worldtransform_.Initialize();
	worldtransform_.translation_ = { 0.0f,2.0f,40.0f };
	worldtransform_.UpdateMatrix();
	obj2_->SetWorldTransform(worldtransform_);

	//obj_->SetSpotLight(spotLight_);
	
	ground_->SetSpotLight(spotLight_);
	// パーティクル
	particle_->SetWorldTransform({.translation_ = obj_->GetWorldPosition()});
	particle_->SetTexture(tex_);
	particle_->SetCamera(camera_.get());
	//particle_->SetMaterial({ .color = {1.0f,1.0f,1.0f,0.8f} });

	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Init();
	postProcess_->SetEffectNo(PostEffectMode::kFullScreen);
	preGravity_ = 0.0f;
	prePower_ = 0.36f;
	preParticleScale_ = {1.0f,1.0f,1.0f};

	material_.color = { 1.0f,1.0f,1.0f,0.8f };
	material_.enableLighting = true;
	material_.uvTransform = MakeIdentity4x4();
	material_.shininess = 60.0f;
	obj2_->SetMaterial(material_);
	particle_->SetMaterial(material_);
	gameBGM_ = Audio::GetInstance()->SoundLoadWave("Resources/game.wav");
	slimeDeadSE_ = Audio::GetInstance()->SoundLoadWave("Resources/bullet.wav");
	Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), gameBGM_, true);
}

void DemoScene::Update()
{
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState()) {
	}

	//short leftStickX = joyState.Gamepad.sThumbLX;
	// 重量加速度
	const float kGravityAcceleration = 0.005f;
	gravity_ += kGravityAcceleration;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0, -gravity_, 0 };
	// 加速する
	move_ = Add(move_, accelerationVector);
	obj_->DemoUpdate();
	ParticleEmitter();
	Vector3 cameraPos = camera_->GetTranslate();
	ImGui::Begin("OBB,BALL");


	ImGui::DragFloat4("sColor", &spotLight_.color.x, 0.1f);
	ImGui::DragFloat3("sDire", &spotLight_.direction.x, 0.1f);
	ImGui::DragFloat3("sPos", &spotLight_.position.x, 0.1f);
	ImGui::DragFloat3("sDis", &spotLight_.distance, 0.1f);
	ImGui::DragFloat("sInten", &spotLight_.intensity, 0.1f);
	ImGui::DragFloat("sDacya", &spotLight_.dacya, 0.1f);

	ImGui::DragFloat3("cameraPos", &cameraPos.x, 0.1f);

	ImGui::Text("playerPosX %f", spotLight_.position.x);
	ImGui::Text("playerPosZ %f", spotLight_.position.z);


	ImGui::End();

	spotLight_.direction = Normalize(spotLight_.direction);
	obj_->SetSpotLight(spotLight_);
	ground_->SetSpotLight(spotLight_);
	ground_->Update();
	
	camera_->SetTranslate(cameraPos);
	camera_->Update();
	
	//PostEffectChange();
	
	obj2_->Update();
	particle_->SetWorldTransform({ .translation_ = obj_->GetWorldPosition() });
	particle_->Update(false);
	postProcess_->Update();
}
void DemoScene::Draw()
{
	particle_->Draw();
	ground_->Draw();
	obj_->Draw(camera_.get());
	//obj2_->Draw(tex_,camera_.get());
	
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
			postProcess_->SetEffectNo(PostEffectMode::kFullScreen);
		}


		ImGui::SliderFloat("hue", &hsv.x, -1.0f, 1.0f);
		ImGui::SliderFloat("saturation", &hsv.y, -1.0f, 1.0f);
		ImGui::SliderFloat("value", &hsv.z, -1.0f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GrayScale")) {
		if (ImGui::Button("GrayScale On")) {
			postProcess_->SetEffectNo(PostEffectMode::kGrayscale);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Viggnet")) {
		if (ImGui::Button("Viggnet On")) {
			postProcess_->SetEffectNo(PostEffectMode::kVignetting);
		}
		ImGui::SliderFloat("darkness 1", &viggnetDarkness.x, 0.0f, 16.0f);
		ImGui::SliderFloat("darkness 2", &viggnetDarkness.y, 0.0f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GaussianFilter")) {
		if (ImGui::Button("Gaussian On")) {
			postProcess_->SetEffectNo(PostEffectMode::kGaussianFilter);
		}
		ImGui::SliderFloat("Devaition", &gauss, 0.01f, 10.0f);
		ImGui::TreePop();
	}




	if (ImGui::TreeNode("DepthOutline")) {
		if (ImGui::Button("DepthOutline On")) {
			postProcess_->SetEffectNo(PostEffectMode::kDepthOutline);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Radial Blur")) {
		if (ImGui::Button("Radial Blur On")) {
			postProcess_->SetEffectNo(PostEffectMode::kRadialBlur);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Dissolve")) {
		if (ImGui::Button("Dissolve On")) {
			postProcess_->SetEffectNo(PostEffectMode::kDissolve);
		}
		ImGui::SliderFloat("Devaition", &threa, 0.00f, 1.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Random")) {
		if (ImGui::Button("Random On")) {
			postProcess_->SetEffectNo(PostEffectMode::kRandom);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Bloom")) {
		if (ImGui::Button("Bloom On")) {
			postProcess_->SetEffectNo(PostEffectMode::kBloom);
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

void DemoScene::ParticleEmitter()
{

	Input::GetInstance()->GetJoystickState();
	if (!obj_->IsEnemyHit()) {
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_A)) {
			obj_->HitEnemySlime();
			Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), slimeDeadSE_, false);
			emitter_.boundPro.power = prePower_;
			emitter_.boundPro.gravity = preGravity_;
			emitter_.boundPro.isBound = true;
			emitter_.size = obj_->GetWorldTransform()->scale_.x;
			particle_->SetEmitter(emitter_);
			particle_->CreateParticle();
		}
	}

	ImGui::Begin("ParticleEmitter");

	ImGui::DragFloat("power", &prePower_, 0.01f);
	ImGui::DragFloat("gravity", &preGravity_,0.01f);
	
	
	if (ImGui::Button("MakeParticle")) {
		obj_->HitEnemySlime();
		
		emitter_.boundPro.power = prePower_;
		emitter_.boundPro.gravity = preGravity_;
		emitter_.boundPro.isBound = true;
		emitter_.size = obj_->GetWorldTransform()->scale_.x;
		particle_->SetEmitter(emitter_);
		particle_->CreateParticle();
	}
	ImGui::End();
	emitter_.transform.scale = { 
		0.25f * obj_->GetWorldTransform()->scale_.x,
		0.25f * obj_->GetWorldTransform()->scale_.x ,
		0.25f * obj_->GetWorldTransform()->scale_.x };




}
