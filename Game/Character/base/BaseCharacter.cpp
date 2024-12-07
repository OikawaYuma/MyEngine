#include "BaseCharacter.h"



void BaseCharacter::Init()
{
	worldTransform_.Initialize();
}

void BaseCharacter::Update()
{
	for (Object3d* object : objects_) {
		object->Update();
	}
	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(Camera* camera)
{
	for (Object3d* object : objects_) {
		object->Draw(camera);
	}
}
