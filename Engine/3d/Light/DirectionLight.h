/**
* @ file DiercitonLight.h
* @ brief 平行光源用の構造体
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Vector4.h"
#include "Vector3.h"

struct DirectionalLight
{
	Vector4 color;
	Vector3 direction;
	float intensity;

};
